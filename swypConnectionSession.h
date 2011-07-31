//
//  swypConnectionSession.h
//  swyp
//
//  Created by Alexander List on 7/27/11.
//  Copyright 2011 ExoMachina. Some rights reserved -- see included 'license' file.
//

#import <Foundation/Foundation.h>
#import "swypCandidate.h"
#import "swypCryptoSession.h"
#import "swypInfoRef.h"
#import "swypFileTypeString.h"
#import "swypConcatenatedInputStream.h"
#import "swypTransformPathwayInputStream.h"
#import "swypInputToOutputStreamConnector.h"
#import "swypFileTypeString.h"

static NSString * const swypConnectionSessionErrorDomain = @"swypConnectionSessionErrorDomain";
typedef enum {
	swypConnectionSessionSocketError
}	swypConnectionSessionErrorCode;


@class swypConnectionSession;

typedef enum {
	swypConnectionSessionStatusNotReady = 0,
	swypConnectionSessionStatusPreparing,
	swypConnectionSessionStatusReady
} swypConnectionSessionStatus;

@protocol swypConnectionSessionInfoDelegate <NSObject>
-(void) sessionDied:			(swypConnectionSession*)session withError:(NSError*)error;

@optional
-(void) sessionWillDie:			(swypConnectionSession*)session;
-(void) sessionStatusChanged:	(swypConnectionSessionStatus)status	inSession:(swypConnectionSession*)session;
@end

@protocol swypConnectionSessionDataDelegate <NSObject>
/*
	Though there are several data delegates, only one delegate should return a stream, all else returning nil
	All delegates will receive the output stream in the finnishedReceiving call
*/
-(NSOutputStream*) streamToWriteReceivedDataWithTag:(NSString*)tag type:(swypFileTypeString*)type length:(NSUInteger)streamLength connectionSession:(swypConnectionSession*)session;
-(void) finishedReceivingDataWithOutputStream:(NSOutputStream*)stream error:(NSError*)error tag:(NSString*)tag type:(swypFileTypeString*)type connectionSession:(swypConnectionSession*)session;
@end


@interface swypConnectionSession : NSObject <NSStreamDelegate> {
	NSMutableSet *	_dataDelegates;
	NSMutableSet *	_connectionSessionInfoDelegates;
	
	UIColor *		_sessionHueColor;
	
	swypCryptoSession *		_cryptoSession;
	swypCandidate *			_representedCandidate;
	
	swypConnectionSessionStatus			_connectionStatus;
	
	swypConcatenatedInputStream *		_sendDataQueueStream;				//setCloseWhenFinished:NO 
	swypTransformPathwayInputStream *	_socketOutputTransformInputStream;	//initWithDataInputStream:_sendDataQueueStream transformStreamArray:nil 
	swypInputToOutputStreamConnector *	_outputStreamConnector;				//initWithOutputStream:_socketOutputStream readStream:_socketOutputTransformInputStream
	
	NSInputStream *			_socketInputStream;
	NSOutputStream *		_socketOutputStream;
}
@property (nonatomic, readonly)	swypConnectionSessionStatus	connectionStatus;
@property (nonatomic, retain)	UIColor*					sessionHueColor;
@property (nonatomic, readonly)	swypCandidate *				representedCandidate;

-(id)	initWithSwypCandidate:	(swypCandidate*)candidate inputStream:(NSInputStream*)inputStream outputStream:(NSOutputStream*)outputStream;

-(void)	invalidate;

-(void)	addDataDelegate:(id<swypConnectionSessionDataDelegate>)delegate;
-(void)	removeDataDelegate:(id<swypConnectionSessionDataDelegate>)delegate;

-(void)	addConnectionSessionInfoDelegate:(id<swypConnectionSessionInfoDelegate>)delegate;
-(void)	removeConnectionSessionInfoDelegate:(id<swypConnectionSessionInfoDelegate>)delegate;

//sending data
/*
	length: the length of the 'stream' property
		if length is specified as 0, the entire stream will be read to memory before any of it can be written (to allow packet size conveyance)
		if length is specified as NSUIntegerMax (UIn!), the stream will be written without a length specifier, to allow devs to do fun stuff
	if there is already a stream sending, this stream will be queued
*/
-(void)	beginSendingFileStreamWithTag:(NSString*)tag  type:(swypFileTypeString*)fileType dataStreamForSend:(NSInputStream*)stream length:(NSUInteger)streamLength;
/* same as above, a convinience method for those who wish to use data already in-memory */
-(void)	beginSendingDataWithTag:(NSString*)tag type:(swypFileTypeString*)type dataForSend:(NSData*)sendData; 


//
//private
-(void)	_changeStatus:	(swypConnectionSessionStatus)status;

@end
