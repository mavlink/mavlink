//
//  MAVLinkTests.swift
//  MAVLinkTests
//
//  Created by Max Odnovolyk on 9/28/16.
//  Copyright © 2016 Build Apps. All rights reserved.
//

import XCTest

@testable import MAVLink

class DataExtensionsTests: XCTestCase {
    
    // MARK: Test number<T: MAVLinkNumber>(at offset: Data.Index) throws -> T
    
    func testGetNumberCannotGetTooLargeNumber() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0])
        
        let test: () throws -> Void = {
            let _: Int32 = try data.number(at: 1)
        }
        
        XCTAssertThrowsError(try test()) { error in
            switch error {
            case let ParseError.valueSizeOutOfBounds(offset, size, upperBound) where offset + size > upperBound:
                break
            default:
                XCTFail("Unexpected error thrown")
            }
        }
    }
    
    func testGetNumberByteOrderIsSwappedOnLittleEndianSystem() {
        let memoryBytes: [UInt8] = [0x04, 0x03, 0x02, 0x01]
        let expectedNumber: UInt32 = 0x01_02_03_04
        
        let data = Data(bytes: memoryBytes)
        let receivedNumber: UInt32 = try! data.number(at: 0, byteOrder: Int(CFByteOrderLittleEndian.rawValue))
        
        XCTAssert(receivedNumber == expectedNumber, "Method expects swapped bytes in memory on little-endian system (most significant digit byte at the end)")
    }
    
    func testGetNumberByteOrderRemainsSameOnBigEndianSystem() {
        let memoryBytes: [UInt8] = [0x01, 0x02, 0x03, 0x04]
        let expectedNumber: UInt32 = 0x01_02_03_04
        
        let data = Data(bytes: memoryBytes)
        let receivedNumber: UInt32 = try! data.number(at: 0, byteOrder: Int(CFByteOrderBigEndian.rawValue))
        
        XCTAssert(receivedNumber == expectedNumber, "Method expects less significant digit byte at the end of value's memory chunk")
    }
    
    // MARK: Test array<T: MAVLinkNumber>(at offset: Data.Index, capacity: Int) throws -> [T]
    
    func testGetArrayCannotAccessBytesOutOfDataBounds() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0])
        
        let test: () throws -> Void = {
            let _: [UInt8] = try data.array(at: 1, capacity: 4)
        }
        
        XCTAssertThrowsError(try test()) { error in
            switch error {
            case let ParseError.valueSizeOutOfBounds(offset, size, upperBound) where offset + size > upperBound:
                break
            default:
                XCTFail("Unexpected error thrown")
            }
        }
    }
    
    func testGetArrayReturnsRequestedNumberOfItems() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0])
        let itemsCountToGet = 3
        
        let array: [UInt8] = try! data.array(at: 1, capacity: itemsCountToGet)
        XCTAssert(array.count == itemsCountToGet, "Expect requested capacity of array to be equal to received array's count")
    }
    
    // MARK: Test string(at offset: Data.Index, length: Int) throws -> String
    
    func testGetStringCannotAccessBytesOutOfDataBounds() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0])
        
        let test: () throws -> Void = {
            let _ = try data.string(at: 1, length: 4)
        }
        
        XCTAssertThrowsError(try test()) { error in
            switch error {
            case let ParseError.valueSizeOutOfBounds(offset, size, upperBound) where offset + size > upperBound:
                break
            default:
                XCTFail("Unexpected error thrown")
            }
        }

    }

    func testGetStringCannotReadNonASCIISEncodedString() {
        // Check that invalidStringEncoding is thrown in case of non ASCII compatible strings
        // It looks like all strings are ASCII compatible for String(bytes: encoding:) method
    }
    
    func testGetStringCanReadEmptyNullTerminatedASCIIEncodedString() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0, 0x0, 0x0]) // "\0\0\0\0\0\0"
        let string = try! data.string(at: 1, length: 4)
        
        XCTAssert(string == "", "Expect to get empty string from zeroed data bytes")
    }
    
    func testGetStringCanReadNullTerminatedASCIIEncodedString() {
        let data = Data(bytes: [0x60, 0x61, 0x62, 0x0, 0x64, 0x65]) // "`ab\0de"
        let string = try! data.string(at: 1, length: 4)
        
        XCTAssert(string == "ab", "Expect cutted off by null-termination string")
    }
    
    func testGetStringCanReadExactlyLengthSizedASCIIEncodedStringWithoutNullTermination() {
        let data = Data(bytes: [0x60, 0x61, 0x62, 0x63, 0x64, 0x65]) // "`abcde"
        let string = try! data.string(at: 1, length: 3)
        
        XCTAssert(string == "abc", "Expect cutted off by length limit string")
    }
    
    // MARK: Test enumeration<T: Enumeration>(at offset: Data.Index) throws -> T where T.RawValue: MAVLinkNumber
    
    func testGetEnumerationCannotInitWithUnknownValue() {
        let data = Data(bytes: [UInt8(ADSBAltitudeType.enumEnd)])
        
        let test: () throws -> Void = {
            let _: ADSBAltitudeType = try data.enumeration(at: 0)
        }
        
        XCTAssertThrowsError(try test()) { error in
            switch error {
            case ParseEnumError<ADSBAltitudeType>.unknownValue:
                break
            default:
                XCTFail("Unexpected error thrown")
            }
        }
    }
    
    func testGetEnumerationReturnsProperCase() {
        let data = Data(bytes: [ADSBAltitudeType.Geometric.rawValue])
        let adsbAltitudeType: ADSBAltitudeType = try! data.enumeration(at: 0)
        
        XCTAssert(adsbAltitudeType == .Geometric, "Expected adsbAltitudeType is .Geometric")
    }
    
    // MARK: Test set<T : MAVLinkNumber>(_ number: T, at offset: Data.Index) throws
    
    func testSetNumberByteOrderIsSwappedOnLittleEndianSystem() {
        
    }
    
    func testSetNumberByteOrderRemainsSameOnBigEndianSystem() {
        
    }
}
