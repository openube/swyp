//
//  swypConnectionSession.h
//  swyp
//
//  Created by Alexander List on 7/27/11.
//  Copyright 2011 ExoMachina. Some rights reserved -- check online.
//

#import <Foundation/Foundation.h>
#import "swypCandidate.h"
#import "swypCryptoSession.h"
#import "swypInfoRef.h"
#import "swypFileTypeString.h"
#import "swypConcatenatedInputStream.h"
#import "swypTransformPathwayInputStream.h"
#import "swypInputToOutputStreamConnector.h"

@class swypConnectionSession;

@protocol swypConnectionSessionDelegate <NSObject>
-(void) sessionIsReady:	(swypConnectionSession*)session;
-(void) sessionWillDie:	(swypConnectionSession*)session;
-(void) sessionDied:	(swypConnectionSession*)session withError:(NSError*)error;
@end

@protocol swypConnectionSessionDataDelegate <NSObject>
-(NSOutputStream*) streamToWriteReceivedDataWithTag:(NSString*)tag type:(swypFileTypeString*)type length:(NSUInteger)streamLength connectionSession:(swypConnectionSession*)session;
-(void) finishedReceivingDataWithOutputStream:(NSOutputStream*)stream error:(NSError*)error tag:(NSString*)tag type:(swypFileTypeString*)type connectionSession:(swypConnectionSession*)session;
@end


@interface swypConnectionSession : NSObject {
	NSMutableSet *	_dataDelegates;
	NSMutableSet *	_connectionSessionDelegates;
	
	swypCryptoSession *		_cryptoSession;
	swypCandidate *			_swypCandidate;
	
	swypConcatenatedInputStream *		_sendDataQueueStream;				//setCloseWhenFinished:NO 
	swypTransformPathwayInputStream *	_socketOutputTransformInputStream;	//initWithDataInputStream:_sendDataQueueStream transformStreamArray:nil 
	swypInputToOutputStreamConnector *	_outputStreamConnector;				//initWithOutputStream:_socketOutputStream readStream:_socketOutputTransformInputStream
	
	NSInputStream *			_socketInputStream;
	NSOutputStream *		_socketOutputStream;
}

-(void)	addDataDelegate:(id<swypConnectionSessionDataDelegate>)delegate;
-(void)	removeDataDelegate:(id<swypConnectionSessionDataDelegate>)delegate;

-(void)	addConnectionSessionDelegate:(id<swypConnectionSessionDelegate>)delegate;
-(void)	removeConnectionSessionDelegate:(id<swypConnectionSessionDelegate>)delegate;


@end