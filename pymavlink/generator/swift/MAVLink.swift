import Foundation


/**
    Common protocol for all MAVLink entities which describes type metadata properties
*/
public protocol MAVLinkEntity: CustomStringConvertible, CustomDebugStringConvertible {
    
    /// Original MAVLink enum name (from declarations xml)
    static var typeName: String { get }
    
    /// Compact type description
    static var typeDescription: String { get }
    
    /// Verbose type description
    static var typeDebugDescription: String { get }
}


// MARK: Enumeration protocol


/**
    Enumeration protocol description with common for all MAVLink enums properties requirements
*/
public protocol Enumeration: RawRepresentable, Equatable, MAVLinkEntity {
    
    /// Array with all members of current enum
    static var allMembers: [Self] { get }
    
    // Array with `Name` - `Description` tuples (values from declarations xml file)
    static var membersDescriptions: [(String, String)] { get }
    
    /// `ENUM_END` flag for checking if enum case value is valid
    static var enumEnd: UInt { get }
    
    /// Original MAVLinks enum member name (as declared in definitions xml file)
    var memberName: String { get }
    
    /// Specific member description from definitions xml
    var memberDescription: String { get }
}


/**
    Enumeration protocol default behaviour implementation
*/
extension Enumeration {
    public static var typeDebugDescription: String {
        let cases = allMembers.map({ $0.debugDescription }).joinWithSeparator("\\n\\t")
        return "Enum \(typeName): \(typeDescription)\\nMembers:\\n\\t\(cases)"
    }
    
    public var description: String {
        return memberName
    }
    
    public var debugDescription: String {
        return "\(memberName): \(memberDescription)"
    }
    
    public var memberName: String {
        return Self.membersDescriptions[Self.allMembers.indexOf(self)!].0
    }
    
    public var memberDescription: String {
        return Self.membersDescriptions[Self.allMembers.indexOf(self)!].1
    }
}


// MARK: Message protocol


/// Message field definition tuple
public typealias FieldDefinition = (name: String, offset: Int, type: String, length: UInt, description: String)


/**
    Message protocol describes common for all MAVLink messages properties and methods requirements
*/
public protocol Message: MAVLinkEntity {
    static var id: UInt8 { get }
    
    static var payloadLength: UInt8 { get }
    
    /// Array of tuples with field definition info
    static var fieldDefinitions: [FieldDefinition] { get }
    
    /// All fields names and values of current Message
    var allFields: [(String, Any)] { get }
    
    /**
        Initialize Message from received data
     
        - Warning: Throws `ParseError` or `ParseEnumError` if any parsing error occurs
    */
    init(data: NSData) throws
    
    /**
        Return NSData representation of current `Message` struct guided by format from `fieldDefinitions`
     
        - Warning: Throws `PackError` if any of message fields does not comply format from `fieldDefinitions`
    */
    func pack() throws -> NSData
}


/**
    Message protocol default behaviour implementation
*/
extension Message {
    public static var payloadLength: UInt8 {
        return messageLengths[id] ?? Packet.Constants.maxPayloadLength
    }
    
    public static var typeDebugDescription: String {
        let fields = fieldDefinitions.map({ "\($0.name): \($0.type): \($0.description)" }).joinWithSeparator("\n\t")
        return "Struct \(typeName): \(typeDescription)\nFields:\n\t\(fields)"
    }
    
    public var description: String {
        let describeField: ((String, Any)) -> String = { (let name, value) in
            let valueString = value is String ? "\"\(value)\"" : value
            return "\(name): \(valueString)"
        }
        let fieldsDescription = allFields.map(describeField).joinWithSeparator(", ")
        return "\(self.dynamicType)(\(fieldsDescription))"
    }
    
    public var debugDescription: String {
        let describeFieldVerbose: ((String, Any)) -> String = { (let name, value) in
            let valueString = value is String ? "\"\(value)\"" : value
            let (_, _, _, _, description) = Self.fieldDefinitions.filter { $0.name == name }.first!
            return "\(name) = \(valueString) : \(description)"
        }
        let fieldsDescription = allFields.map(describeFieldVerbose).joinWithSeparator("\n\t")
        return "\(Self.typeName): \(Self.typeDescription)\nFields:\n\t\(fieldsDescription)"
    }
    
    public var allFields: [(String, Any)] {
        var result: [(String, Any)] = []
        let mirror = Mirror(reflecting: self)
        for case let (label?, value) in mirror.children {
            result.append((label, value))
        }
        return result
    }
}


/**
    MAVLink type aliases
*/
public typealias Channel = UInt8


// MARK: Error enumerations


/**
    Parsing errors
*/
public enum ParseError: ErrorType {
    
    /**
        Size of expected number is larger than receivers' data length
        - offset: Expected number offset in received data
        - size: Expected number size in bytes
    */
    case NumberSizeOutOfBounds(offset: Int, size: Int)
    
    /**
        Length check of payload for known `messageId` failed
        - messageId: ID of expected `Message` type
        - receivedLength: Received payload length
        - properLength: Expected payload length for `Message` type
    */
    case InvalidPayloadLength(messageId: UInt8, receivedLength: UInt8, properLength: UInt8)
    
    /**
        Received `messageId` was not recognized so we can't create appropirate `Message`
        - messageId: Id of the message that was not found in known message list (`messageIdToClass` array)
    */
    case UnknownMessageId(messageId: UInt8)
    
    /// Checksum check failed. Message id is known but calculated CRC bytes does not match received CRC value
    case BadCRC
}


/**
    Special error type for returning Enum parsing errors with details in associated values (types of this values are not compatible with `ParseError` enum)
*/
public enum ParseEnumError<T: RawRepresentable>: ErrorType {
    
    /**
        Enumeration case with `rawValue` at `valueOffset` was not fouund in `enumType` enumeration
        - enumType: Type of expected enumeration
        - rawValue: Raw value that was not found in `enumType`
        - valueOffset: Value offset in received payload data
    */
    case UnknownValue(enumType: T.Type, rawValue: T.RawValue, valueOffset: Int)
}


/**
    Errors that can occur while packing `Message` for sending
*/
public enum PackError: ErrorType {
    
    /**
        Size of received value (together with offset) is out of receivers' lenght
        - offset: Expected value offset in payload
        - size: Provided field value size in bytes
        - length: Available payload length
    */
    case ValueSizeOutOfBounds(offset: Int, size: Int, length: Int)
    
    /**
        Length check for provided field value failed
        - offset: Expected value offset in payload
        - providedValueLength: Count of elements (characters) in provided value
        - allowedLength: Maximum number of elements (characters) allowed in field
    */
    case InvalidValueLength(offset: Int, providedValueLength: Int, allowedLength: Int)
    
    /**
        String field contains non ASCII characters
        - offset: Expected value offset in payload
    */
    case InvalidEncoding(offset: Int)
    
    /**
        Failed to allocate `NSMutableData` for payload or packet buffer
        - length: Length of required buffer
    */
    case BufferAllocationFailed(lenght: Int)
    
    /**
        CRC extra byte not found for provided `messageId` type
        - messageId: ID of message type
    */
    case CRCExtraNotFound(messageId: UInt8)
    
    /**
        Packet finalization process failed due to `message` absence
    */
    case MessageNotSet
}


/**
    Alternative way to receive parsed Messages, finalized packets' data and all errors is to implement this protocol and set as `MAVLink`s' delegate
*/
public protocol MAVLinkDelegate {
    
    /**
        Called when MAVLink packet is successfully received, payload length and CRC checks are passed
     
        - Parameter link: `MAVLink` object that handled `packet`
        - Parameter packet: Completely received `Packet`
        - Parameter channel: Channel on which `packet` was received
     
        - Returns: `Void`
    */
    func link(link: MAVLink, didReceivePacket packet: Packet, channel: Channel)
    
    /**
        Packet receiving failed due to `InvalidPayloadLength` or `BadCRC` error
     
        - Parameter link: `MAVLink` object that received `data`
        - Parameter error: Error that  occurred while receiving `data` (`InvalidPayloadLength` or `BadCRC` error)
        - Parameter data: Partially received `Packet`
        - Parameter channel: Channel on which `data` was received
     
        - Returns: `Void`
    */
    func link(link: MAVLink, didFailToReceivePacketWithError error: ErrorType, data: Packet?, channel: Channel)
    
    /**
        Called when received data was successfully parsed into appropriate `message` structure
     
        - Parameter link: `MAVLink` object that handled `packet`
        - Parameter message: Successfully parsed `Message`
        - Parameter packet: Completely received `Packet`
        - Parameter channel: Channel on which `message` was received
     
        - Returns: `Void`
    */
    func link(link: MAVLink, didParseMessage message: Message, fromPacket packet: Packet, channel: Channel)
    
    /**
        Called when `packet` completely received but `MAVLink` was not able to finish `Message` processing due unknown `messageId` or type validation errors
     
        - Parameter link: `MAVLink` object that handled `packet`
        - Parameter packet: Completely received `Packet`
        - Parameter error: Error that  occurred while parsing `packet`s payload into `Message`
        - Parameter channel: Channel on which `message` was received
     
        - Returns: `Void`
    */
    func link(link: MAVLink, didFailToParseMessageFromPacket packet: Packet, withError error: ErrorType, channel: Channel)
    
    /**
        Called when message is finalized and ready for sending to aircraft
     
        - Parameter link: `MAVLink` object that handled `message`
        - Parameter message: Message to be sent
        - Parameter data: Compiled data that represents `message`
        - Parameter channel: Channel on which `message` should be sent
     
        - Returns: `Void`
    */
    func link(link: MAVLink, didFinalizeMessage: Message, data: NSData, channel: Channel)
}


// MARK: MAVLink class implementation


/**
    Main MAVLink class, performs `Packet` receiving, recognition, validation, `Message` structure creation and `Message` packing, finalizing for sending. Also returns errors through delegation if any errors occurred.

    - Warning: Supports only 1.0 version of the MAVlink wire protocol.
*/
public class MAVLink {
    
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
    
    /// Storage for MAVLink parsed packets count, states and errors statistics
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
    
    /// MAVLink Packets and States buffers
    let channelBuffers = (0 ..< Channel.max).map({ _ in Packet() })
    let channelStatuses = (0 ..< Channel.max).map({ _ in Status() })
    
    /// Object to pass received packets, messages, errors, finalized data to
    public var delegate: MAVLinkDelegate?
    
    /// Enable this option to check the length of each message. This allows invalid messages to be caught much sooner. Use if the transmission medium is prone to missing (or extra) characters (e.g. a radio that fades in and out). Only use if the channel will only contain messages types listed in the headers.
    public var checkMessageLength = true
    
    /// Use one extra CRC that is added to the message CRC to detect mismatches in message specifications. This is to prevent that two devices using different message versions incorrectly decode a message with the same length. Defined as `let` as we support only the latest version (1.0) of the MAVLink wire protocol.
    public let crcExtra = true
    
    /**
        This is a convenience function which handles the complete MAVLink parsing. The function will parse one byte at a time and return the complete packet once it could be successfully decoded. Checksum and other failures will be delegated to `delegate`.
     
        - Parameter char: The char to parse
        - Parameter chan: ID of the current channel. This allows to parse different channels with this function. A channel is not a physical message channel like a serial port, but a logic partition of the communication streams in this case.
     
        - Returns: `nil` if no packet could be decoded at the moment, the `Packet` structure else
    */
    public func parseChar(char: UInt8, channel: Channel) -> Packet? {
        
        /// Function to check if current char is Stx byte. If current char is STX, modifies current rxpack and status.
        func handleStxChar(char: UInt8, rxpack: Packet, status: Status) {
            if char == Packet.Constants.packetStx {
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
                    status.parseError += 1
                    status.parseState = .Idle
                    let error = ParseError.InvalidPayloadLength(messageId: char, receivedLength: rxpack.length, properLength: messageLength)
                    delegate?.link(self, didFailToReceivePacketWithError: error, data: nil, channel: channel)
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
            if crcExtra && (messageCRCsExtra[rxpack.messageId] != nil) {
                rxpack.checksum.accumulateChar(messageCRCsExtra[rxpack.messageId]!)
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
                status.parseError += 1
                status.packetReceived = .BadCRC
                let error = messageIdToClass[rxpack.messageId] == nil ? ParseError.UnknownMessageId(messageId: rxpack.messageId) : ParseError.BadCRC
                delegate?.link(self, didFailToReceivePacketWithError: error, data: Packet(packet: rxpack), channel: channel)
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
        delegate?.link(self, didReceivePacket: packet, channel: channel)
        
        status.currentRxSeq = rxpack.sequence
        // Initial condition: If no packet has been received so far, drop count is undefined
        if status.packetRxSuccessCount == 0 {
            status.packetRxDropCount = 0;
        }
        // Count this packet as received
        status.packetRxSuccessCount = status.packetRxSuccessCount &+ 1
        
        // Try to create appropriate Message structure, delegate results
        guard let messageClass = messageIdToClass[packet.messageId] else {
            let error = ParseError.UnknownMessageId(messageId: rxpack.messageId)
            delegate?.link(self, didFailToParseMessageFromPacket: packet, withError: error, channel: channel)
            return packet
        }
        
        do {
            packet.message = try messageClass.init(data: rxpack.payload)
            delegate?.link(self, didParseMessage: packet.message!, fromPacket: packet, channel: channel)
        } catch {
            delegate?.link(self, didFailToParseMessageFromPacket: packet, withError: error, channel: channel)
            return packet
        }
        
        return packet
    }
    
    /**
        Parse new portion of data, then call `messageHandler` if new message is available
     
        - Parameter data: The data to be parsed
        - Parameter channel: ID of the current channel. This allows to parse different channels with this function. A channel is not a physical message channel like a serial port, but a logic partition of the communication streams in this case.
        - Parameter messageHandler: The message handler to call when the provided data is enought to complete message parsing. Unless you have provided a custom delegate, this parameter must not be `nil`, because there is no other way to retrieve the parsed message and packet.
     
        - Returns: `Void`
    */
    public func parseData(data: NSData, channel: Channel, messageHandler:((message: Message, packet: Packet) -> Void)? = nil) {
        let bytes = UnsafeBufferPointer<UInt8>(start: UnsafePointer<UInt8>(data.bytes), count:data.length)
        bytes.forEach { char in
            if let packet = parseChar(char, channel: channel), message = packet.message, messageHandler = messageHandler {
                messageHandler(message: message, packet: packet)
            }
        }
    }
    
    /**
        Prepare `message`s' bytes for sending, pass to `delegate` for further processing and increase sequence counter.
     
        - Parameter message: Message to be compiled into bytes and sent
        - Parameter systemId: ID of the sending (this) system
        - Parameter componentId: ID of the sending component
        - Parameter channel: ID of the current channel
     
        - Warning: Throws `PackError`
     
        - Returns: `Void`
    */
    public func dispatchMessage(message: Message, systemId: UInt8, componentId: UInt8, channel: Channel) throws {
        let channelStatus = channelStatuses[Int(channel)]
        let packet = Packet(message: message, systemId: systemId, componentId: componentId, channel: channel)
        let data = try packet.finalize(sequence: channelStatus.currentTxSeq)
        delegate?.link(self, didFinalizeMessage: message, data: data, channel: channel)
        channelStatus.currentTxSeq = channelStatus.currentTxSeq &+ 1
    }
}


/**
    MAVLink Packet structure to store received data that is not full message yet. Contains additional to Message info like channel, system id, component id and raw payload data, etc. Also used to store and transfer received data of unknown or corrupted Messages. [More details](http://qgroundcontrol.org/mavlink/start)
*/
public class Packet {
    
    /// MAVlink Packet constants
    struct Constants {
        
        /// Maximum packets payload length
        static let maxPayloadLength = UInt8.max
        
        static let numberOfChecksumBytes = 2
        
        /// Length of core header (of the comm. layer): message length (1 byte) + message sequence (1 byte) + message system id (1 byte) + message component id (1 byte) + message type id (1 byte)
        static let coreHeaderLength = 5
        
        /// Length of all header bytes, including core and checksum
        static let numberOfHeaderBytes = Constants.numberOfChecksumBytes + Constants.coreHeaderLength + 1
        
        /// Packet start sign. Indicates the start of a new packet. v1.0
        static let packetStx: UInt8 = 0xFE
    }
    
    /// Channel on which packet was received
    public internal(set) var channel: UInt8 = 0
    
    /// Sent at the end of packet
    public internal(set) var checksum = Checksum()
    
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
    
    /// ID of message type in payload
    public internal(set) var messageId: UInt8 = 0
    
    /// Message bytes
    public internal(set) var payload: NSMutableData = NSMutableData(capacity: Int(Constants.maxPayloadLength) + Constants.numberOfChecksumBytes)!
    
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
    
    /**
        Initialize packet with provided `message` for sending
     
        - Parameter message: Message to send
        - Parameter systemId: ID of the sending (this) system
        - Parameter componentId: ID of the sending component
        - Parameter channel: ID of the current channel
    */
    init(message: Message, systemId: UInt8, componentId: UInt8, channel: Channel) {
        self.magic = Constants.packetStx
        self.systemId = systemId
        self.componentId = componentId
        self.messageId = message.dynamicType.id
        self.length = message.dynamicType.payloadLength
        self.message = message
        self.channel = channel
    }
    
    init() { }
    
    /**
        Finalize a MAVLink packet with sequence assignment. Returns data that could be sent to the aircraft. This function calculates the checksum and sets length and aircraft id correctly. It assumes that the packet is already correctly initalized with apropirate `message`, `length`, `systemId`, `componentId`.
        Could be used to send packets without `MAVLink` object, in this case you should take care of `sequence` counter manually.
     
        - Parameter sequence: Each channel counts up his send sequence. Allows to detect packet loss
     
        - Warning: Throws `PackError`
     
        - Returns: `NSData`
    */
    public func finalize(sequence sequence: UInt8) throws -> NSData {
        guard let message = message else {
            throw PackError.MessageNotSet
        }
        
        guard let crcExtra = messageCRCsExtra[messageId] else {
            throw PackError.CRCExtraNotFound(messageId: message.dynamicType.id)
        }
        
        self.sequence = sequence
        
        let payload = try message.pack()
        let coreHeader = [length, sequence, systemId, componentId, messageId]
        
        checksum.start()
        checksum.accumulateBuffer(coreHeader)
        checksum.accumulateBuffer(payload)
        checksum.accumulateChar(crcExtra)
        
        let packetDataLength = payload.length + Constants.numberOfHeaderBytes
        guard let packetData = NSMutableData(capacity: packetDataLength) else {
            throw PackError.BufferAllocationFailed(lenght: packetDataLength)
        }
        
        var headerBytes = [Constants.packetStx] + coreHeader
        packetData.appendBytes(&headerBytes, length: headerBytes.count * strideofValue(UInt8))
        packetData.appendData(payload)
        
        var checksumBytes = [checksum.lowByte, checksum.highByte]
        packetData.appendBytes(&checksumBytes, length: checksumBytes.count * strideofValue(UInt8))
        
        return packetData
    }
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
    
    /**
        Accumulate the X.25 CRC by adding `buffer` bytes
     
        - Parameter buffer: Bytes to hash
     
        - Returns: `Void`
    */
    mutating func accumulateBuffer(buffer: NSData) {
        let bytes = UnsafeBufferPointer<UInt8>(start: UnsafePointer<UInt8>(buffer.bytes), count:buffer.length)
        bytes.forEach({ accumulateChar($0) })
    }
    
    /**
        Accumulate the X.25 CRC by adding `buffer` array
     
        - Parameter buffer: Array of bytes to hash
     
        - Returns: `Void`
    */
    mutating func accumulateBuffer(buffer: [UInt8]) {
        buffer.forEach({ accumulateChar($0) })
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
     
        - Warning: Supports only version 1.0 of MAVLink wire protocol (little-endian), throws `ParserError`
     
        - Returns: `T`
    */
    func mavNumber<T>(offset offset: Int) throws -> T {
        let size = strideof(T)
        guard offset + size <= length else {
            throw ParseError.NumberSizeOutOfBounds(offset: offset, size: size)
        }
        
        var bytes = [UInt8](count: size, repeatedValue: 0)
        getBytes(&bytes, range: NSRange(location: offset, length: size))
        
        if CFByteOrderGetCurrent() != Int(CFByteOrderLittleEndian.rawValue) {
            bytes = bytes.reverse()
        }
        
        return bytes.withUnsafeBufferPointer({ UnsafePointer<T>($0.baseAddress).memory })
    }
    
    /**
        Returns typed array from data
     
        - Parameter offset: Offset in data bytes
        - Parameter count: Number of elements in array
     
        - Warning: Throws `ParserError`
     
        - Returns: `Array<T>`
    */
    func mavArray<T>(offset offset: Int, count: Int) throws -> Array<T> {
        var offset = offset
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
        var buffer = [CChar](count: length + 1, repeatedValue: 0x00)
        getBytes(&buffer, range: NSRange(location: offset, length: length))
        let string = NSString(CString: &buffer, encoding: NSASCIIStringEncoding) ?? ""
        return string as String
    }
    
    /**
        Returns proper Enum from data or throws `UnknownEnumValue` error
     
        - Parameter offset: Offset in data bytes
     
        - Warning: Throws `ParserEnumError`
     
        - Returns: `T: RawRepresentable`
    */
    func mavEnumeration<T: RawRepresentable>(offset offset: Int) throws -> T {
        let int: T.RawValue = try mavNumber(offset: offset)
        if let enumeration = T(rawValue: int) {
            return enumeration
        }
        throw ParseEnumError.UnknownValue(enumType: T.self, rawValue: int, valueOffset: offset)
    }
}


/**
    Methods for filling payload with correctly formatted field values
*/
extension NSMutableData {
    
    /**
        Set properly swapped `number` bytes starting from `offset` in receivers' bytes
     
        - Parameter number: Number value to set
        - Parameter offset: Position in receivers' bytes
     
        - Warning: Supports only version 1.0 of MAVLink wire protocol (little-endian), throws `ValueSizeOutOfBounds`
    */
    func setMAVNumber<T>(number: T, offset: Int) throws {
        let size = strideof(T)
        guard offset + size <= length else {
            throw PackError.ValueSizeOutOfBounds(offset: offset, size: size, length: length)
        }
        
        var number = number
        let range = NSRange(location: offset, length: size)
        
        if CFByteOrderGetCurrent() != Int(CFByteOrderLittleEndian.rawValue) {
            return withUnsafePointer(&number) {
                var bytes = Array<UInt8>(UnsafeBufferPointer(start: UnsafePointer<UInt8>($0), count: size))
                bytes = bytes.reverse()
                bytes.withUnsafeBufferPointer({ replaceBytesInRange(range, withBytes: $0.baseAddress) })
            }
        } else {
            return withUnsafePointer(&number, { replaceBytesInRange(range, withBytes: $0) })
        }
    }
    
    /**
        Set `array` of values at `offset` validating `allowedCount`
     
        - Parameter array: Array of values to set
        - Parameter offset: Position in receivers' bytes
        - Parameter allowedCount: Maximum allowed count of elements in `array`
     
        - Warning: Throws `ValueSizeOutOfBounds`, `InvalidValueLength`
    */
    func setMAVArray<T>(array: Array<T>, offset: Int, allowedCount: Int) throws {
        guard array.count <= allowedCount else {
            throw PackError.InvalidValueLength(offset: offset, providedValueLength: array.count, allowedLength: allowedCount)
        }
        
        let elementSize = strideof(T)
        let arraySize = elementSize * array.count
        
        guard offset + arraySize <= length else {
            throw PackError.ValueSizeOutOfBounds(offset: offset, size: arraySize, length: length)
        }
        
        let range = NSRange(location: offset, length: arraySize)
        array.withUnsafeBufferPointer({ replaceBytesInRange(range, withBytes: $0.baseAddress) })
    }
    
    /**
        Set correctly encoded `string` value at `offset` limited to `allowedLength` or throws `PackError`.
        Adds NULL termination if actual length of `string` is less than `allowedLength`.
     
        - Precondition: `String` value must be ASCII compatible
     
        - Parameter string: Value to set
        - Parameter offset: Position in receivers' bytes
        - Parameter allowedLength: Maximum allowed length of string
     
        - Warning: Throws `InvalidEncoding`, `ValueSizeOutOfBounds`, `InvalidValueLength`
    */
    func setMAVString(string: String, offset: Int, allowedLength: Int) throws {
        guard var bytes = string.cStringUsingEncoding(NSASCIIStringEncoding) else {
            throw PackError.InvalidEncoding(offset: offset)
        }
        
        // Remove ending NULL if string length is exactly `allowedLength`
        if bytes.count > allowedLength {
            bytes.removeLast()
        }
        
        try setMAVArray(bytes, offset: offset, allowedCount: allowedLength)
    }
    
    /**
        Set correctly formated `enumeration` raw value at `offset` or throws `PackError`
     
        - Parameter enumeration: Value to set
        - Parameter offset: Position in receivers' bytes
     
        - Warning: Throws `ValueSizeOutOfBounds`
    */
    func setMAVEnumeration<T: Enumeration>(enumeration: T, offset: Int) throws {
        try setMAVNumber(enumeration.rawValue, offset: offset)
    }
}


// MARK: Additional MAVLink service info