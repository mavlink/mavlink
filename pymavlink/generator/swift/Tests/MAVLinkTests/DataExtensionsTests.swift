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
        
        let testClosure: () throws -> Void = {
            let _: Int32 = try data.number(at: 1)
        }
        
        XCTAssertThrowsError(try testClosure()) { error in
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
    
    func testGetArrayCannotAccessBytesOutOfDataBound() {
        let data = Data(bytes: [0x0, 0x0, 0x0, 0x0])
        
        let testClosure: () throws -> Void = {
            let _: [UInt8] = try data.array(at: 1, capacity: 4)
        }
        
        XCTAssertThrowsError(try testClosure()) { error in
            switch error {
            case let ParseError.valueSizeOutOfBounds(offset, size, upperBound) where offset + size > upperBound:
                break
            default:
                XCTFail("Unexpected error thrown")
            }
        }
    }
    
    func testGetArrayReturnsRequestedNumberOfItems() {
        
    }
    
    // MARK: Test set<T : MAVLinkNumber>(_ number: T, at offset: Data.Index) throws
    
    func testSetNumberByteOrderIsSwappedOnLittleEndianSystem() {
        
    }
    
    func testSetNumberByteOrderRemainsSameOnBigEndianSystem() {
        
    }
}
