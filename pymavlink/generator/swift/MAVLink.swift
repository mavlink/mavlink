import Foundation

/// Common protocol for all MAVLink entities which describes types
/// metadata properties.
public protocol MAVLinkEntity: CustomStringConvertible, CustomDebugStringConvertible {
    
    /// Original MAVLink enum name (from declarations xml)
    static var typeName: String { get }
    
    /// Compact type description
    static var typeDescription: String { get }
    
    /// Verbose type description
    static var typeDebugDescription: String { get }
}

// MARK: Enumeration protocol

/// Enumeration protocol description with common for all MAVLink enums
/// properties requirements.
public protocol Enumeration: RawRepresentable, Equatable, MAVLinkEntity {
    
    /// Array with all members of current enum
    static var allMembers: [Self] { get }
    
    // Array with `Name` - `Description` tuples (values from declarations xml file)
    static var membersDescriptions: [(String, String)] { get }
    
    /// `ENUM_END` flag for checking if enum case value is valid
    static var enumEnd: UInt { get }
    
    /// Original MAVLinks enum member name (as declared in definition’s xml file)
    var memberName: String { get }
    
    /// Specific member description from definitions xml
    var memberDescription: String { get }
}

/// Enumeration protocol default behaviour implementation.
extension Enumeration {
    public static var typeDebugDescription: String {
        let cases = allMembers.map({ $0.debugDescription }).joined(separator: "\\n\\t")
        return "Enum \(typeName): \(typeDescription)\\nMembers:\\n\\t\(cases)"
    }
    
    public var description: String {
        return memberName
    }
    
    public var debugDescription: String {
        return "\(memberName): \(memberDescription)"
    }
    
    public var memberName: String {
        return Self.membersDescriptions[Self.allMembers.index(of: self)!].0
    }
    
    public var memberDescription: String {
        return Self.membersDescriptions[Self.allMembers.index(of: self)!].1
    }
}

// MARK: Message protocol

/// Message field definition tuple.
public typealias FieldDefinition = (name: String, offset: Int, type: String, length: UInt, description: String)

/// Message protocol describes common for all MAVLink messages properties and
/// methods requirements.
public protocol Message: MAVLinkEntity {
    static var id: UInt8 { get }
    
    static var payloadLength: UInt8 { get }
    
    /// Array of tuples with field definition info
    static var fieldDefinitions: [FieldDefinition] { get }
    
    /// All fields names and values of current Message
    var allFields: [(String, Any)] { get }
    
    /// Initialize Message from received data.
    ///
    /// - parameter data: Data to decode.
    ///
    /// - throws: Throws `ParseError` or `ParseEnumError` if any parsing error
    /// occurs.
    init(data: Data) throws
    
    /// Returns `Data` representation of current `Message` struct guided
    /// by format from `fieldDefinitions`.
    ///
    /// - throws: Throws `PackError` if any of message fields does not comply
    /// format from `fieldDefinitions`.
    ///
    /// - returns: Receiver’s `Data` representation
    func pack() throws -> Data
}

/// Message protocol default behaviour implementation.
extension Message {
    public static var payloadLength: UInt8 {
        return messageLengths[id] ?? Packet.Constant.maxPayloadLength
    }
    
    public static var typeDebugDescription: String {
        let fields = fieldDefinitions.map({ "\($0.name): \($0.type): \($0.description)" }).joined(separator: "\n\t")
        return "Struct \(typeName): \(typeDescription)\nFields:\n\t\(fields)"
    }
    
    public var description: String {
        let describeField: ((String, Any)) -> String = { (name, value) in
            let valueString = value is String ? "\"\(value)\"" : value
            return "\(name): \(valueString)"
        }
        let fieldsDescription = allFields.map(describeField).joined(separator: ", ")
        return "\(type(of: self))(\(fieldsDescription))"
    }
    
    public var debugDescription: String {
        let describeFieldVerbose: ((String, Any)) -> String = { (name, value) in
            let valueString = value is String ? "\"\(value)\"" : value
            let (_, _, _, _, description) = Self.fieldDefinitions.filter { $0.name == name }.first!
            return "\(name) = \(valueString) : \(description)"
        }
        let fieldsDescription = allFields.map(describeFieldVerbose).joined(separator: "\n\t")
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

// MARK: MAVLink type aliases

public typealias Channel = UInt8

protocol MAVLinkNumber { }

extension UInt8: MAVLinkNumber { }

extension Int8: MAVLinkNumber { }

extension UInt16: MAVLinkNumber { }

extension Int16: MAVLinkNumber { }

extension UInt32: MAVLinkNumber { }

extension Int32: MAVLinkNumber { }

extension UInt64: MAVLinkNumber { }

extension Int64: MAVLinkNumber { }

extension Float: MAVLinkNumber { }

extension Double: MAVLinkNumber { }

// MARK: Error enumerations

/// Parsing errors
public enum ParseError: Error {
    
    /// Size of expected number is larger than receivers' data length.
    /// - offset: Expected number offset in received data.
    /// - size: Expected number size in bytes.
    /// - upperBound: The number of bytes in the data.
    case valueSizeOutOfBounds(offset: Int, size: Int, upperBound: Int)
    
    /// Data contains non ASCII characters.
    /// - offset: String offset in received data.
    /// - length: Expected length of string to read.
    case invalidStringEncoding(offset: Int, length: Int)
    
    /// Length check of payload for known `messageId` failed.
    /// - messageId: ID of expected `Message` type.
    /// - receivedLength: Received payload length.
    /// - properLength: Expected payload length for `Message` type.
    case invalidPayloadLength(messageId: UInt8, receivedLength: UInt8, properLength: UInt8)
    
    /// Received `messageId` was not recognized so we can't create appropirate
    /// `Message`.
    /// - messageId: Id of the message that was not found in known message
    /// list (`messageIdToClass` array).
    case unknownMessageId(messageId: UInt8)
    
    /// Checksum check failed. Message id is known but calculated CRC bytes
    /// does not match received CRC value.
    case badCRC
}

/// Special error type for returning Enum parsing errors with details in associated
/// values (types of this values are not compatible with `ParseError` enum).
public enum ParseEnumError<T: RawRepresentable>: Error {
    
    /// Enumeration case with `rawValue` at `valueOffset` was not fouund in
    /// `enumType` enumeration.
    /// - enumType: Type of expected enumeration.
    /// - rawValue: Raw value that was not found in `enumType`.
    /// - valueOffset: Value offset in received payload data.
    case unknownValue(enumType: T.Type, rawValue: T.RawValue, valueOffset: Int)
}

/// Errors that can occur while packing `Message` for sending.
public enum PackError: Error {
    
    /// Size of received value (together with offset) is out of receiver’s lenght.
    /// - offset: Expected value offset in payload.
    /// - size: Provided field value size in bytes.
    /// - upperBound: Available payload length.
    case valueSizeOutOfBounds(offset: Int, size: Int, upperBound: Int)
    
    /// Length check for provided field value failed.
    /// - offset: Expected value offset in payload.
    /// - providedValueLength: Count of elements (characters) in provided value.
    /// - allowedLength: Maximum number of elements (characters) allowed in field.
    case invalidValueLength(offset: Int, providedValueLength: Int, allowedLength: Int)
    
    /// String field contains non ASCII characters.
    /// - offset: Expected value offset in payload.
    /// - string: Original string.
    case invalidStringEncoding(offset: Int, string: String)
    
    /// CRC extra byte not found for provided `messageId` type.
    /// - messageId: ID of message type.
    case crcExtraNotFound(messageId: UInt8)
    
    /// Packet finalization process failed due to `message` absence.
    case messageNotSet
}

// MARK: Delegate protocol

/// Alternative way to receive parsed Messages, finalized packets' data and all
/// errors is to implement this protocol and set as `MAVLink`s' delegate.
public protocol MAVLinkDelegate {
    
    /// Called when MAVLink packet is successfully received, payload length check
    /// and CRC are passed.
    ///
    /// - parameter link:    `MAVLink` object that handled `packet`.
    /// - parameter packet:  Completely received `Packet`.
    /// - parameter channel: Channel on which `packet` was received.
    func link(_ link: MAVLink, didReceivePacket packet: Packet, channel: Channel)
    
    
    /// Packet receiving failed due to `InvalidPayloadLength` or `BadCRC` error.
    ///
    /// - parameter link:    `MAVLink` object that received `data`.
    /// - parameter error:   Error that  occurred while receiving `data`
    /// (`InvalidPayloadLength` or `BadCRC` error).
    /// - parameter data:    Partially received `Packet`.
    /// - parameter channel: Channel on which `data` was received.
    func link(_ link: MAVLink, didFailToReceivePacketWithError error: Error, data: Packet?, channel: Channel)
    
    /// Called when received data was successfully parsed into appropriate
    /// `message` structure.
    ///
    /// - parameter link:    `MAVLink` object that handled `packet`.
    /// - parameter message: Successfully parsed `Message`.
    /// - parameter packet:  Completely received `Packet`.
    /// - Parameter channel: Channel on which `message` was received.
    func link(_ link: MAVLink, didParseMessage message: Message, fromPacket packet: Packet, channel: Channel)
    
    /// Called when `packet` completely received but `MAVLink` was not able to
    /// finish `Message` processing due unknown `messageId` or type validation
    /// errors.
    ///
    /// - parameter link:    `MAVLink` object that handled `packet`.
    /// - parameter packet:  Completely received `Packet`.
    /// - parameter error:   Error that  occurred while parsing `packet`s
    /// payload into `Message`.
    /// - parameter channel: Channel on which `message` was received.
    func link(_ link: MAVLink, didFailToParseMessageFromPacket packet: Packet, withError error: Error, channel: Channel)
    
    /// Called when message is finalized and ready for sending to aircraft.
    ///
    /// - parameter link:    `MAVLink` object that handled `message`.
    /// - parameter message: Message to be sent.
    /// - parameter data:    Compiled data that represents `message`.
    /// - parameter channel: Channel on which `message` should be sent.
    func link(_ link: MAVLink, didFinalizeMessage: Message, data: Data, channel: Channel)
}

// MARK: MAVLink class implementation

/// Main MAVLink class, performs `Packet` receiving, recognition, validation,
/// `Message` structure creation and `Message` packing, finalizing for sending.
/// Also returns errors through delegation if any errors occurred.
/// - warning: Supports only 1.0 version of the MAVlink wire protocol.
public class MAVLink {
    
    /// States for the parsing state machine.
    enum ParseState {
        case uninit
        case idle
        case gotStx
        case gotSequence
        case gotLength
        case gotSystemId
        case gotComponentId
        case gotMessageId
        case gotPayload
        case gotCRC1
        case gotBadCRC1
    }
    
    enum Framing: UInt8 {
        case incomplete = 0
        case ok = 1
        case badCRC = 2
    }
    
    /// Storage for MAVLink parsed packets count, states and errors statistics.
    class Status {
        
        /// Number of received packets
        var packetReceived: Framing = .incomplete
        
        /// Number of parse errors
        var parseError: UInt8 = 0
        
        /// Parsing state machine
        var parseState: ParseState = .uninit
        
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
    
    /// Object to pass received packets, messages, errors, finalized data to.
    public var delegate: MAVLinkDelegate?
    
    /// Enable this option to check the length of each message. This allows
    /// invalid messages to be caught much sooner. Use if the transmission
    /// medium is prone to missing (or extra) characters (e.g. a radio that
    /// fades in and out). Only use if the channel will only contain messages
    /// types listed in the headers.
    public var checkMessageLength = true
    
    /// Use one extra CRC that is added to the message CRC to detect mismatches
    /// in message specifications. This is to prevent that two devices using
    /// different message versions incorrectly decode a message with the same
    /// length. Defined as `let` as we support only the latest version (1.0) of
    /// the MAVLink wire protocol.
    public let crcExtra = true
    
    /// This is a convenience function which handles the complete MAVLink
    /// parsing. The function will parse one byte at a time and return the
    /// complete packet once it could be successfully decoded. Checksum and
    /// other failures will be delegated to `delegate`.
    ///
    /// - parameter char:    The char to parse.
    /// - parameter channel: ID of the current channel. This allows to parse
    /// different channels with this function. A channel is not a physical
    /// message channel like a serial port, but a logic partition of the
    /// communication streams in this case.
    ///
    /// - returns: Returns `nil` if packet could be decoded at the moment,
    /// the `Packet` structure else.
    public func parse(char: UInt8, channel: Channel) -> Packet? {
        
        /// Function to check if current char is Stx byte. If current char is
        /// STX, modifies current rxpack and status.
        func handleSTX(char: UInt8, rxpack: Packet, status: Status) {
            if char == Packet.Constant.packetStx {
                rxpack.length = 0
                rxpack.channel = channel
                rxpack.magic = char
                rxpack.checksum.start()
                status.parseState = .gotStx
            }
        }
        
        let rxpack = channelBuffers[Int(channel)]
        let status = channelStatuses[Int(channel)]
        
        status.packetReceived = .incomplete
        
        switch status.parseState {
        case .uninit, .idle:
            handleSTX(char: char, rxpack: rxpack, status: status)
            
        case .gotStx:
            rxpack.length = char
            rxpack.payload.count = 0
            rxpack.checksum.accumulate(char: char)
            status.parseState = .gotLength
            
        case .gotLength:
            rxpack.sequence = char
            rxpack.checksum.accumulate(char: char)
            status.parseState = .gotSequence
            
        case .gotSequence:
            rxpack.systemId = char
            rxpack.checksum.accumulate(char: char)
            status.parseState = .gotSystemId
            
        case .gotSystemId:
            rxpack.componentId = char
            rxpack.checksum.accumulate(char: char)
            status.parseState = .gotComponentId
            
        case .gotComponentId:
            // Check Message length if `checkMessageLength` enabled and
            // `messageLengths` contains proper id. If `messageLengths` does not
            // contain info for current messageId, parsing will fail later on CRC check.
            if checkMessageLength && (messageLengths[char] != nil) {
                if let messageLength = messageLengths[char], rxpack.length != messageLength {
                    status.parseError += 1
                    status.parseState = .idle
                    let error = ParseError.invalidPayloadLength(messageId: char, receivedLength: rxpack.length, properLength: messageLength)
                    delegate?.link(self, didFailToReceivePacketWithError: error, data: nil, channel: channel)
                    break
                }
            }
            rxpack.messageId = char
            rxpack.checksum.accumulate(char: char)
            if rxpack.length == 0 {
                status.parseState = .gotPayload
            } else {
                status.parseState = .gotMessageId
            }
            
        case .gotMessageId:
            rxpack.payload.append(char)
            rxpack.checksum.accumulate(char: char)
            if rxpack.payload.count == Int(rxpack.length) {
                status.parseState = .gotPayload
            }
            
        case .gotPayload:
            if crcExtra && (messageCRCsExtra[rxpack.messageId] != nil) {
                rxpack.checksum.accumulate(char: messageCRCsExtra[rxpack.messageId]!)
            }
            if char != rxpack.checksum.lowByte {
                status.parseState = .gotBadCRC1
            } else {
                status.parseState = .gotCRC1
            }
            rxpack.payload.append(char)
            
        case .gotCRC1, .gotBadCRC1:
            if (status.parseState == .gotBadCRC1) || (char != rxpack.checksum.highByte) {
                status.parseError += 1
                status.packetReceived = .badCRC
                let error = messageIdToClass[rxpack.messageId] == nil ? ParseError.unknownMessageId(messageId: rxpack.messageId) : ParseError.badCRC
                delegate?.link(self, didFailToReceivePacketWithError: error, data: Packet(packet: rxpack), channel: channel)
            } else {
                // Successfully got message
                rxpack.payload.append(char)
                status.packetReceived = .ok
            }
            status.parseState = .idle
        }
        
        defer {
            // Сollect stat here
            
            status.parseError = 0
        }
        
        // If a packet has been sucessfully received
        guard status.packetReceived == .ok else {
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
            let error = ParseError.unknownMessageId(messageId: rxpack.messageId)
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
    
    /// Parse new portion of data, then call `messageHandler` if new message
    /// is available.
    ///
    /// - parameter data:           Data to be parsed.
    /// - parameter channel:        ID of the current channel. This allows to
    /// parse different channels with this function. A channel is not a physical
    /// message channel like a serial port, but a logic partition of the
    /// communication streams in this case.
    /// - parameter messageHandler: The message handler to call when the
    /// provided data is enought to complete message parsing. Unless you have
    /// provided a custom delegate, this parameter must not be `nil`, because
    /// there is no other way to retrieve the parsed message and packet.
    public func parse(data: Data, channel: Channel, messageHandler:((Message, Packet) -> Void)? = nil) {
        data.forEach { byte in
            if let packet = parse(char: byte, channel: channel), let message = packet.message, let messageHandler = messageHandler {
                messageHandler(message, packet)
            }
        }
    }
    
    /// Prepare `message` bytes for sending, pass to `delegate` for further
    /// processing and increase sequence counter.
    ///
    /// - parameter message:     Message to be compiled into bytes and sent.
    /// - parameter systemId:    ID of the sending (this) system.
    /// - parameter componentId: ID of the sending component.
    /// - parameter channel:     ID of the current channel.
    ///
    /// - throws: Throws `PackError`.
    public func dispatchMessage(message: Message, systemId: UInt8, componentId: UInt8, channel: Channel) throws {
        let channelStatus = channelStatuses[Int(channel)]
        let packet = Packet(message: message, systemId: systemId, componentId: componentId, channel: channel)
        let data = try packet.finalize(sequence: channelStatus.currentTxSeq)
        delegate?.link(self, didFinalizeMessage: message, data: data, channel: channel)
        channelStatus.currentTxSeq = channelStatus.currentTxSeq &+ 1
    }
}

/// MAVLink Packet structure to store received data that is not full message yet.
/// Contains additional to Message info like channel, system id, component id
/// and raw payload data, etc. Also used to store and transfer received data of
/// unknown or corrupted Messages.
/// [More details](http://qgroundcontrol.org/mavlink/start).
public class Packet {
    
    /// MAVlink Packet constants
    struct Constant {
        
        /// Maximum packets payload length
        static let maxPayloadLength = UInt8.max
        
        static let numberOfChecksumBytes = 2
        
        /// Length of core header (of the comm. layer): message length
        /// (1 byte) + message sequence (1 byte) + message system id (1 byte) +
        /// message component id (1 byte) + message type id (1 byte).
        static let coreHeaderLength = 5
        
        /// Length of all header bytes, including core and checksum
        static let numberOfHeaderBytes = Constant.numberOfChecksumBytes + Constant.coreHeaderLength + 1
        
        /// Packet start sign. Indicates the start of a new packet. v1.0.
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
    public internal(set) var payload = Data(capacity: Int(Constant.maxPayloadLength) + Constant.numberOfChecksumBytes)
    
    /// Received Message structure if available
    public internal(set) var message: Message?
    
    /// Initialize copy of provided Packet.
    ///
    /// - parameter packet: Packet to copy
    init(packet: Packet) {
        channel = packet.channel
        checksum = packet.checksum
        magic = packet.magic
        length = packet.length
        sequence = packet.sequence
        systemId = packet.systemId
        componentId = packet.componentId
        messageId = packet.messageId
        payload = packet.payload
        message = packet.message
    }
    
    /// Initialize packet with provided `message` for sending.
    ///
    /// - parameter message:     Message to send.
    /// - parameter systemId:    ID of the sending (this) system.
    /// - parameter componentId: ID of the sending component.
    /// - parameter channel:     ID of the current channel.
    init(message: Message, systemId: UInt8, componentId: UInt8, channel: Channel) {
        self.magic = Constant.packetStx
        self.systemId = systemId
        self.componentId = componentId
        self.messageId = type(of: message).id
        self.length = type(of: message).payloadLength
        self.message = message
        self.channel = channel
    }
    
    init() { }
    
    /// Finalize a MAVLink packet with sequence assignment. Returns data that
    /// could be sent to the aircraft. This function calculates the checksum and
    /// sets length and aircraft id correctly. It assumes that the packet is
    /// already correctly initalized with apropirate `message`, `length`,
    /// `systemId`, `componentId`.
    /// Could be used to send packets without `MAVLink` object, in this case you
    /// should take care of `sequence` counter manually.
    ///
    /// - parameter sequence: Each channel counts up his send sequence. Allows
    /// to detect packet loss.
    ///
    /// - throws: Throws `PackError`.
    ///
    /// - returns: Data
    public func finalize(sequence: UInt8) throws -> Data {
        guard let message = message else {
            throw PackError.messageNotSet
        }
        
        guard let crcExtra = messageCRCsExtra[messageId] else {
            throw PackError.crcExtraNotFound(messageId: type(of: message).id)
        }
        
        self.sequence = sequence
        
        let coreHeader = [length, sequence, systemId, componentId, messageId]
        let header = [Constant.packetStx] + coreHeader
        let payload = try message.pack()
        let checksumBytes = [checksum.lowByte, checksum.highByte]
        
        checksum.start()
        checksum.accumulate(buffer: coreHeader)
        checksum.accumulate(buffer: payload)
        checksum.accumulate(char: crcExtra)
        
        var packetData = Data(capacity: payload.count + Constant.numberOfHeaderBytes)
        packetData.append(header, count: header.count)
        packetData.append(payload)
        packetData.append(checksumBytes, count: checksumBytes.count)
        
        return packetData
    }
}

/// Struct for storing and calculating checksum.
public struct Checksum {
    
    struct Constants {
        static let x25InitCRCValue: UInt16 = 0xFFFF
    }
    
    public var lowByte: UInt8 {
        return UInt8(truncatingBitPattern: value)
    }
    
    public var highByte: UInt8 {
        return UInt8(truncatingBitPattern: value >> 8)
    }
    
    public private(set) var value: UInt16 = 0
    
    init() {
        start()
    }
    
    /// Initiliaze the buffer for the X.25 CRC.
    mutating func start() {
        value = Constants.x25InitCRCValue
    }
    
    /// Accumulate the X.25 CRC by adding one char at a time. The checksum
    /// function adds the hash of one char at a time to the 16 bit checksum
    /// `value` (`UInt16`).
    ///
    /// - parameter char: New char to hash
    mutating func accumulate(char: UInt8) {
        var tmp: UInt8 = char ^ UInt8(truncatingBitPattern: value)
        tmp ^= (tmp << 4)
        value = (UInt16(value) >> 8) ^ (UInt16(tmp) << 8) ^ (UInt16(tmp) << 3) ^ (UInt16(tmp) >> 4)
    }
    
    /// Accumulate the X.25 CRC by adding `buffer` bytes.
    ///
    /// - parameter buffer: Sequence of bytes to hash
    mutating func accumulate<T: Sequence>(buffer: T) where T.Iterator.Element == UInt8 {
        buffer.forEach { accumulate(char: $0) }
    }
}

// MARK: Data extensions

/// Methods for getting properly typed field values from received data.
extension Data {
    
    /// Returns number value (integer or floating point) from receiver’s data.
    ///
    /// - parameter offset: Offset in receiver’s bytes.
    ///
    /// - throws: Throws `ParseError`.
    ///
    /// - returns: Returns `MAVLinkNumber` (UInt8, Int8, UInt16, Int16, UInt32,
    /// Int32, UInt64, Int64, Float, Double).
    func number<T: MAVLinkNumber>(at offset: Data.Index) throws -> T {
        let size = MemoryLayout<T>.stride
        let range: Range<Int> = offset ..< offset + size
        
        guard range.upperBound <= count else {
            throw ParseError.valueSizeOutOfBounds(offset: offset, size: size, upperBound: count)
        }
        
        var bytes = subdata(in: range)
        if CFByteOrderGetCurrent() != Int(CFByteOrderLittleEndian.rawValue) {
            bytes.reverse()
        }
        
        return bytes.withUnsafeBytes { $0.pointee }
    }
    
    /// Returns typed array from receiver’s data.
    ///
    /// - parameter offset:   Offset in receiver’s bytes.
    /// - parameter capacity: Expected number of elements in array.
    ///
    /// - throws: Throws `ParseError`.
    ///
    /// - returns: `Array<T>`
    func array<T: MAVLinkNumber>(at offset: Data.Index, capacity: Int) throws -> [T] {
        var offset = offset
        var array = [T]()
        
        for _ in 0 ..< capacity {
            array.append(try number(at: offset))
            offset += MemoryLayout<T>.stride
        }
        
        return array
    }
    
    /// Returns ASCII String from receiver’s data.
    ///
    /// - parameter offset: Offset in receiver’s bytes.
    /// - parameter length: Expected length of string to read.
    ///
    /// - throws: Throws `ParseError`.
    ///
    /// - returns: `String`
    func string(at offset: Data.Index, length: Int) throws -> String {
        let range: Range<Int> = offset ..< offset + length
        let bytes = subdata(in: range)
        
        guard range.upperBound <= count else {
            throw ParseError.valueSizeOutOfBounds(offset: offset, size: length, upperBound: count)
        }
        
        guard let string = String(bytes: bytes, encoding: .ascii) else {
            throw ParseError.invalidStringEncoding(offset: offset, length: length)
        }
        
        return string
    }
    
    /// Returns proper typed `Enumeration` subtype value from data or throws
    /// `ParserEnumError` or `ParseError` error.
    ///
    /// - parameter offset: Offset in receiver’s bytes.
    ///
    /// - throws: Throws `ParserEnumError`, `ParseError`.
    ///
    /// - returns: Properly typed `Enumeration` subtype value.
    func enumeration<T: Enumeration>(at offset: Data.Index) throws -> T where T.RawValue: MAVLinkNumber {
        let rawValue: T.RawValue = try number(at: offset)
        
        guard let enumerationCase = T(rawValue: rawValue) else {
            throw ParseEnumError.unknownValue(enumType: T.self, rawValue: rawValue, valueOffset: offset)
        }
        
        return enumerationCase
    }
}

/// Methods for filling `Data` with properly formatted field values.
extension Data {
    
    /// Sets properly swapped `number` bytes starting from `offset` in
    /// receiver's bytes.
    ///
    /// - warning: Supports only version 1.0 of MAVLink wire protocol
    /// (little-endian byte order).
    ///
    /// - parameter number: Number value to set.
    /// - parameter offset: Offset in receiver’s bytes.
    ///
    /// - throws: Throws `PackError`.
    mutating func set<T: MAVLinkNumber>(_ number: T, at offset: Data.Index) throws -> Void {
        let size = MemoryLayout<T>.stride
        let range = offset ..< offset + size
        
        guard range.endIndex <= count else {
            throw PackError.valueSizeOutOfBounds(offset: offset, size: size, upperBound: count)
        }
        
        var number = number
        var bytes: Data = withUnsafePointer(to: &number) { numberPointer in
            let bufferPointer = numberPointer.withMemoryRebound(to: UInt8.self, capacity: size) { pointer in
                UnsafeBufferPointer(start: pointer, count: size)
            }
            return Data(bufferPointer)
        }
        
        
        if CFByteOrderGetCurrent() != Int(CFByteOrderLittleEndian.rawValue) {
            bytes.reverse()
        }
        
        replaceSubrange(range, with: bytes)
    }
    
    /// Sets `array` of `MAVLinkNumber` values at `offset` with `capacity` validation.
    ///
    /// - parameter array:    Array of values to set.
    /// - parameter offset:   Offset in receiver’s bytes.
    /// - parameter capacity: Maximum allowed count of elements in `array`.
    ///
    /// - throws: Throws `PackError`.
    mutating func set<T: MAVLinkNumber>(_ array: [T], at offset: Data.Index, capacity: Int) throws {
        guard array.count <= capacity else {
            throw PackError.invalidValueLength(offset: offset, providedValueLength: array.count, allowedLength: capacity)
        }
        
        let elementSize = MemoryLayout<T>.stride
        let arraySize = elementSize * array.count
        
        guard offset + arraySize <= count else {
            throw PackError.valueSizeOutOfBounds(offset: offset, size: arraySize, upperBound: count)
        }
        
        for (index, item) in array.enumerated() {
            try set(item, at: index * elementSize)
        }
    }
    
    /// Sets correctly encoded `string` value at `offset` limited to `length` or
    /// throws `PackError`.
    ///
    /// - precondition: `string` value must be ASCII compatible.
    ///
    /// - parameter string: Value to set.
    /// - parameter offset: Offset in receiver’s bytes.
    /// - parameter length: Maximum allowed length of `string`.
    ///
    /// - throws: Throws `PackError`.
    mutating func set(_ string: String, at offset: Data.Index, length: Int) throws {
        guard var bytes = string.data(using: .ascii) else {
            throw PackError.invalidStringEncoding(offset: offset, string: string)
        }
        
        // Remove possible string’s null-termination
        if bytes.count > length {
            bytes.removeLast()
        }
        
        let asciiCharacters = bytes.withUnsafeBytes { Array(UnsafeBufferPointer<UInt8>(start: $0, count: bytes.count)) }
        try set(asciiCharacters, at: offset, capacity: length)
    }
    
    /// Sets correctly formated `enumeration` raw value at `offset` or throws
    /// `PackError`.
    ///
    /// - parameter enumeration: Value to set.
    /// - parameter offset:      Offset in receiver’s bytes.
    ///
    /// - throws: Throws `PackError`.
    mutating func set<T: Enumeration>(_ enumeration: T, at offset: Data.Index) throws where T.RawValue: MAVLinkNumber {
        try set(enumeration.rawValue, at: offset)
    }
}

// MARK: Additional MAVLink service info
