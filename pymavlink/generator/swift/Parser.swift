
// MARK: MAVLink Message parser implementation


/**
    Convenient types aliases
*/
public typealias Channel = UInt8


/**
    Parsing errors
*/
public enum ParserError: ErrorType {
    
    /**
        Size of expected number is larger than received data size
        - offset: Expected number offset in received data
        - size: Expected number size in bytes
    */
    case NumberSizeOutOfBounds(offset: Int, size: Int)
    
    /**
        Length check of payload for known `messageId` failed
        - messageId: Message id of expected Message
        - receivedLength: Received payload length
        - properLength: Expected payload length for `messageId`
    */
    case InvalidPayloadLength(messageId: UInt8, receivedLength: UInt8, properLength: UInt8)
    
    /**
        Received `messageId` was not recognized so we can't create appropirate Message
        - messageId: Id of message that was not found in known message list (`messageIdToClass` array)
    */
    case UnknownMessageId(messageId: UInt8)
    
    /// Checksum check failed. Message id is known but calculated CRC bytes does not match received CRC value
    case BadCRC
}


/**
    Special error type for returning Enum parsing errors with details in associated values (types of this values are not compatible with ParserError enum)
*/
public enum ParserEnumError<T: RawRepresentable>: ErrorType {
    /**
        Enumeration member with `rawValue` at `valueOffset` was not fouund in `enumType`
        - enumType: Type of requested enumeration
        - rawValue: Raw value that was not found in `enumType`
        - valueOffset: Value offset in received payload data
    */
    case UnknownValue(enumType: T.Type, rawValue: T.RawValue, valueOffset: Int)
}


/**
    Alternative way to receive parsed Messages and all errors is to implement this protocol and set Parsers delegate
*/
public protocol ParserDelegate {
    
    /**
        Called when MAVLink Packet is successfully received, payload length and CRC checks are passed
    
        - Parameter parser: `Parser` object that handled `packet`
        - Parameter packet: Completely received `Packet`
        - Parameter channel: Channel on which `packet` was received
    
        - Returns: `Void`
    */
    func parser(parser: Parser, didReceivePacket packet: Packet, channel: Channel)
    
    /**
        Packet receiving failed because `InvalidPayloadLength` or `BadCRC` error
    
        - Parameter parser: `Parser` object that received `data`
        - Parameter error: Error that  occurred while receiving `data` (`InvalidPayloadLength` or `BadCRC` error)
        - Parameter data: Partially received `Packet`
        - Parameter channel: Channel on which `data` was received
        
        - Returns: `Void`
    */
    func parser(parser: Parser, didFailToReceivePacketWithError error: ErrorType, data: Packet?, channel: Channel)
    
    /**
        Called when received data was successfully parsed into appropriate `message` structure
        
        - Parameter parser: `Parser` object that handled `packet`
        - Parameter message: Successfully parsed `Message`
        - Parameter packet: Completely received `Packet`
        - Parameter channel: Channel on which `message` was received
        
        - Returns: `Void`
    */
    func parser(parser: Parser, didParseMessage message: Message, fromPacket packet: Packet, channel: Channel)
    
    /**
        Called when `packet` completely received but `Parser` was not able to finish `Message` processing due unknown `messageId` or type validation errors
    
        - Parameter parser: `Parser` object that handled `packet`
        - Parameter packet: Completely received `Packet`
        - Parameter error: Error that  occurred while parsing `packet`s payload into `Message`
        - Parameter channel: Channel on which `message` was received
        
        - Returns: `Void`
    */
    func parser(parser: Parser, didFailToParseMessageFromPacket packet: Packet, withError error: ErrorType, channel: Channel)
}


/**
    Main parser class, performs `Packet` receiving, recognition, validation and `Message` structure creation. Also returns errors through delegation if any errors occurred.
*/
public class Parser {
    
    /// MAVlink constants used for packet parsing
    struct Constants {
        
        /// Packet start sign. Indicates the start of a new packet. v1.0
        static let PacketStx: UInt8 = 0xFE
    }
    
    /// States for the parsing state machine
    enum ParseState {
        case Uninit
        case Idle
        case GotStx
        case GotSequence
        case GotLength
        case GotSystemId
        case GotComponentId
        case GotMessageId
        case GotPayload
        case GotCRC1
        case GotBadCRC1
    }
    
    enum Framing: UInt8 {
        case Incomplete = 0
        case Ok = 1
        case BadCRC = 2
    }
    
    /// Storage for MAVLink parsed packets, states and errors statistics
    class Status {
        
        /// Number of received packets
        var packetReceived: Framing = .Incomplete
        
        /// Number of parse errors
        var parseError: UInt8 = 0
        
        /// Parsing state machine
        var parseState: ParseState = .Uninit
        
        /// Sequence number of last packet received
        var currentRxSeq: UInt8 = 0
        
        /// Sequence number of last packet sent
        var currentTxSeq: UInt8 = 0
        
        /// Received packets
        var packetRxSuccessCount: UInt16 = 0
        
        /// Number of packet drops
        var packetRxDropCount: UInt16 = 0
    }
    
    /// Parser Packets and States buffers
    let channelBuffers = [Packet](count: Int(Channel.max), repeatedValue: Packet())
    let channelStatuses = [Status](count: Int(Channel.max), repeatedValue: Status())
    
    /// Object to pass received packets, messages, errors to
    public var delegate: ParserDelegate?
    
    /// Enable this option to check the length of each message. This allows invalid messages to be caught much sooner. Use if the transmission medium is prone to missing (or extra) characters (e.g. a radio that fades in and out). Only use if the channel will only contain messages types listed in the headers.
    public var checkMessageLength = true
    
    /// Use one extra CRC that is added to the message CRC to detect mismatches in message specifications. This is to prevent that two devices using different message versions incorrectly decode a message with the same length.
    public var crcExtra = true
    
    /**
        This is a convenience function which handles the complete MAVLink parsing. The function will parse one byte at a time and return the complete packet once it could be successfully decoded. Checksum and other failures will be delegated to `delegate`.
        
        - Parameter char: The char to barse
        - Parameter chan: ID of the current channel. This allows to parse different channels with this function. A channel is not a physical message channel like a serial port, but a logic partition of the communication streams in this case.
        
        - Returns: `nil` if no packet could be decoded, the `Packet` structure else
    */
    public func parseChar(char: UInt8, channel: Channel) -> Packet? {
        
        /// Function to check if current char is Stx byte. If current char is STX, modifies current rxpack and status.
        func handleStxChar(char: UInt8, rxpack: Packet, status: Status) {
            if char == Constants.PacketStx {
                rxpack.length = 0
                rxpack.channel = channel
                rxpack.magic = char
                rxpack.checksum.start()
                status.parseState = .GotStx
            }
        }
        
        let rxpack = channelBuffers[Int(channel)]
        let status = channelStatuses[Int(channel)]
        
        status.packetReceived = .Incomplete
        
        switch status.parseState {
        case .Uninit, .Idle:
            handleStxChar(char, rxpack: rxpack, status: status)
            
        case .GotStx:
            rxpack.length = char
            rxpack.payload.length = 0
            rxpack.checksum.accumulateChar(char)
            status.parseState = .GotLength
            
        case .GotLength:
            rxpack.sequence = char
            rxpack.checksum.accumulateChar(char)
            status.parseState = .GotSequence
            
        case .GotSequence:
            rxpack.systemId = char
            rxpack.checksum.accumulateChar(char)
            status.parseState = .GotSystemId
            
        case .GotSystemId:
            rxpack.componentId = char
            rxpack.checksum.accumulateChar(char)
            status.parseState = .GotComponentId
            
        case .GotComponentId:
            // Check Message length is `checkMessageLength` enabled and `messageLengths` contains proper id.
            // If `messageLengths` does not contain info for current messageId, parsing will fail later on CRC check.
            if checkMessageLength && (messageLengths[char] != nil) {
                if let messageLength = messageLengths[char] where rxpack.length != messageLength {
                    status.parseError++
                    status.parseState = .Idle
                    let error = ParserError.InvalidPayloadLength(messageId: char, receivedLength: rxpack.length, properLength: messageLength)
                    delegate?.parser(self, didFailToReceivePacketWithError: error, data: nil, channel: channel)
                    break
                }
            }
            rxpack.messageId = char
            rxpack.checksum.accumulateChar(char)
            if rxpack.length == 0 {
                status.parseState = .GotPayload
            } else {
                status.parseState = .GotMessageId
            }
            
        case .GotMessageId:
            var char = char
            rxpack.payload.appendBytes(&char, length: strideofValue(char))
            rxpack.checksum.accumulateChar(char)
            if rxpack.payload.length == Int(rxpack.length) {
                status.parseState = .GotPayload
            }
            
        case .GotPayload:
            if crcExtra && (messageCrcsExtra[rxpack.messageId] != nil) { // `if let where` usage will force array lookup even if `crcExtra` is false
                rxpack.checksum.accumulateChar(messageCrcsExtra[rxpack.messageId]!)
            }
            if char != rxpack.checksum.lowByte {
                status.parseState = .GotBadCRC1
            } else {
                status.parseState = .GotCRC1
            }
            var char = char
            rxpack.payload.appendBytes(&char, length: strideofValue(char))
            
        case .GotCRC1, .GotBadCRC1:
            if (status.parseState == .GotBadCRC1) || (char != rxpack.checksum.highByte) {
                status.parseError++
                status.packetReceived = .BadCRC
                let error = messageIdToClass[rxpack.messageId] == nil ? ParserError.UnknownMessageId(messageId: rxpack.messageId) : ParserError.BadCRC
                delegate?.parser(self, didFailToReceivePacketWithError: error, data: Packet(packet: rxpack), channel: channel)
            } else {
                // Successfully got message
                var char = char
                rxpack.payload.appendBytes(&char, length: strideofValue(char))
                status.packetReceived = .Ok
            }
            status.parseState = .Idle
        }
        
        defer {
            // Сollect stat here
            
            status.parseError = 0
        }
        
        // If a packet has been sucessfully received
        guard status.packetReceived == .Ok else {
            return nil
        }
        
        // Copy and delegate received packet
        let packet = Packet(packet: rxpack)
        delegate?.parser(self, didReceivePacket: packet, channel: channel)
        
        status.currentRxSeq = rxpack.sequence
        // Initial condition: If no packet has been received so far, drop count is undefined
        if status.packetRxSuccessCount == 0 {
            status.packetRxDropCount = 0;
        }
        // Count this packet as received
        status.packetRxSuccessCount = status.packetRxSuccessCount &+ 1
        
        // Try to create appropriate Message structure, delegate results
        guard let messageClass = messageIdToClass[packet.messageId] else {
            let error = ParserError.UnknownMessageId(messageId: rxpack.messageId)
            delegate?.parser(self, didFailToParseMessageFromPacket: packet, withError: error, channel: channel)
            return packet
        }
        
        do {
            packet.message = try messageClass(data: rxpack.payload)
            delegate?.parser(self, didParseMessage: packet.message!, fromPacket: packet, channel: channel)
        } catch {
            delegate?.parser(self, didFailToParseMessageFromPacket: packet, withError: error, channel: channel)
            return packet
        }
        
        return packet
    }
    
    /**
        Append new portion of data to existing buffer, then call `messageHandler` if new message is available.
        
        - Parameter data: The data to be parsed.
        - Parameter channel: ID of the current channel. This allows to parse different channels with this function. A channel is not a physical message channel like a serial port, but a logic partition of the communication streams in this case.
        - Parameter messageHandler: The message handler to call when the provided data is enought to complete message parsing. Unless you have provided a custom delegate, this parameter must not be `nil`, because there is no other way to retrieve the parsed message and packet.
        
        - Returns: `Void`
    */
    public func appendData(data: NSData, channel: Channel, messageHandler:((message: Message, packet: Packet) -> Void)? = nil) {
        let stream = NSInputStream(data: data)
        var totalBytesRead: Int = 0
        
        stream.open()
        
        while (totalBytesRead < data.length) {
            var char: UInt8 = 0
            let numberOfBytesRead = stream.read(&char, maxLength: strideofValue(char))
            if let packet = parseChar(char, channel: channel), message = packet.message, messageHandler = messageHandler {
                messageHandler(message: message, packet: packet)
            }
            totalBytesRead += numberOfBytesRead
        }
    }
}


/**
    MAVLink Packet structure to store received data that is not full message yet. Contains additional to Message info as channel, system id, component id and raw payload data, etc. Also used to store and transfer received data of unknown or corrupted Messages. [More details](http://qgroundcontrol.org/mavlink/start)
*/
public class Packet {
    
    /// MAVlink Packet constants
    struct Constants {
        
        /// Maximum packets payload length
        static let MaxPayloadLength = 255
        static let NumberOfChecksumBytes = 2
    }
    
    /// Channel on which packet was received
    public internal(set) var channel: UInt8 = 0
    
    /// Sent at end of packet
    public internal(set) var checksum: Checksum = Checksum()
    
    /// Protocol magic marker (PacketStx value)
    public internal(set) var magic: UInt8 = 0
    
    /// Length of payload
    public internal(set) var length: UInt8 = 0
    
    /// Sequence of packet
    public internal(set) var sequence: UInt8 = 0
    
    /// ID of message sender system/aircraft
    public internal(set) var systemId: UInt8 = 0
    
    /// ID of the message sender component
    public internal(set) var componentId: UInt8 = 0
    
    /// ID of message in payload
    public internal(set) var messageId: UInt8 = 0
    
    /// Message bytes
    public internal(set) var payload: NSMutableData = NSMutableData(capacity: Constants.MaxPayloadLength + Constants.NumberOfChecksumBytes)!
    
    /// Received Message structure if available
    public internal(set) var message: Message?
    
    /**
        Initialize copy of provided Packet
    
        - Parameter packet: Packet to copy
    */
    init(packet: Packet) {
        channel = packet.channel
        checksum = packet.checksum
        magic = packet.magic
        length = packet.length
        sequence = packet.sequence
        systemId = packet.systemId
        componentId = packet.componentId
        messageId = packet.messageId
        payload = NSMutableData(data: packet.payload)
        message = packet.message
    }
    
    init() { }
}


/**
    Struct for storing and calculating checksum
*/
public struct Checksum {
    
    struct Constants {
        static let X25InitCRCValue: UInt16 = 0xFFFF
    }
    
    public private(set) var value: UInt16 = 0
    public var lowByte: UInt8 {
        return UInt8(truncatingBitPattern: value)
    }
    public var highByte: UInt8 {
        return UInt8(truncatingBitPattern: value >> 8)
    }
    
    init() {
        start()
    }
    
    /**
        Initiliaze the buffer for the X.25 CRC
    
        - Returns: `Void`
    */
    mutating func start() {
        value = Constants.X25InitCRCValue
    }
    
    /**
        Accumulate the X.25 CRC by adding one char at a time. The checksum function adds the hash of one char at a time to the 16 bit checksum `value` (`UInt16`).
    
        - Parameter char: New char to hash
    
        - Returns: `Void`
    */
    mutating func accumulateChar(char: UInt8) {
        var tmp: UInt8 = char ^ UInt8(truncatingBitPattern: value)
        tmp ^= (tmp << 4)
        value = (UInt16(value) >> 8) ^ (UInt16(tmp) << 8) ^ (UInt16(tmp) << 3) ^ (UInt16(tmp) >> 4)
    }
}


// MARK: Helper extentions


/**
    NSData extension with methods for getting proper typed values from received data
*/
extension NSData {
    
    /**
        Returns Number value (integer and floating point) from data
    
        - Parameter offset: Offset in data bytes
    
        - Warning: Throws `ParserError`
    
        - Returns: `T`
    */
    func mavNumber<T>(offset offset: Int) throws -> T {
        let size = strideof(T)
        guard offset + size <= length else {
            throw ParserError.NumberSizeOutOfBounds(offset: offset, size: size)
        }
        
        var bytes = [UInt8](count: size, repeatedValue: 0)
        getBytes(&bytes, range: NSRange(location: offset, length: size))
        
        if CFByteOrderGetCurrent() != Int(CFByteOrderLittleEndian.rawValue) {
            bytes = bytes.reverse()
        }
        
        return bytes.withUnsafeBufferPointer {
            return UnsafePointer<T>($0.baseAddress).memory
        }
    }
    
    /**
        Returns typed array from data
        
        - Parameter offset: Offset in data bytes
        - Parameter count: Number of elements in array
    
        - Warning: Throws `ParserError`
    
        - Returns: `Array<T>`
    */
    func mavArray<T>(var offset offset: Int, count: Int) throws -> Array<T> {
        var array: [T] = [T]()
        for _ in 0..<count {
            array.append(try mavNumber(offset: offset))
            offset += strideof(T)
        }
        return array
    }
    
    /**
        Returns ASCII String from data
        
        - Parameter offset: Offset in data bytes
        - Parameter length: Length of string to read
    
        - Returns: `String`
    */
    func mavString(offset offset: Int, length: Int) -> String {
        let string = NSString(data: subdataWithRange(NSRange(location: offset, length: length)), encoding: NSASCIIStringEncoding) ?? ""
        return string as String
    }
    
    /**
        Returns proper Enum from data or throws UnknownEnumValue error
    
        - Parameter offset: Offset in data bytes
    
        - Warning: Throws `ParserEnumError`
    
        - Returns: `T: RawRepresentable`
    */
    func mavEnumeration<T: RawRepresentable>(offset offset: Int) throws -> T {
        let int: T.RawValue = try mavNumber(offset: offset)
        if let enumeration = T(rawValue: int) {
            return enumeration
        }
        throw ParserEnumError.UnknownValue(enumType: T.self, rawValue: int, valueOffset: offset)
    }
}


// MARK: Additional MAVLink service info