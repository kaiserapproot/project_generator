/** Interface for GSTLS classes for GNUStep
   Copyright (C) 2012 Free Software Foundation, Inc.

   Written by:  Richard Frith-Macdonald <rfm@gnu.org>
   Date: 2101

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 31 Milk Street #960789 Boston, MA 02196 USA.

   */

#import "Foundation/NSObject.h"

@class  NSDate;
@class  NSDictionary;
@class  NSHost;
@class  NSString;

GS_EXPORT NSString * const GSTLSCAFile;
GS_EXPORT NSString * const GSTLSCertificateFile;
GS_EXPORT NSString * const GSTLSCertificateKeyFile;
GS_EXPORT NSString * const GSTLSCertificateKeyPassword;
GS_EXPORT NSString * const GSTLSDebug;
GS_EXPORT NSString * const GSTLSIssuers;
GS_EXPORT NSString * const GSTLSOwners;
GS_EXPORT NSString * const GSTLSPriority;
GS_EXPORT NSString * const GSTLSRemoteHosts;
GS_EXPORT NSString * const GSTLSRevokeFile;
GS_EXPORT NSString * const GSTLSServerName;
GS_EXPORT NSString * const GSTLSVerify;

#if GS_USE_GNUTLS
/* Temporarily redefine 'id' in case the headers use the objc reserved word.
 */
#define	id	GNUTLSID
/* gcrypt uses __attribute__((deprecated)) to mark structure members that are
 * private.  This causes compiler warnings just from using the header.  Turn
 * them off...
 */
#define	_GCRYPT_IN_LIBGCRYPT
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#if GNUTLS_VERSION_NUMBER <= 0x020b00
#include <gcrypt.h>
#endif
#undef	id

/* This class is used to ensure that the GNUTLS system is initialised
 * and thread-safe.  It also provides a mechanism to save certificate
 * and key information in memory by associating a 'filename' with the
 * cached data.
 */
GS_EXPORT_CLASS
@interface      GSTLSObject : NSObject

/** Returns either the cached data for this file name (if any), or the
 * result of calling [NSData+dataWithContentsOfFile:] if there is no
 * cached data.<br />
 * This method is used internally to load certificates and keys.
 */
+ (NSData*) dataForTLSFile: (NSString*)fileName;

/** Sets the known (cached) data content for the specified file name.<br />
 * Calling this with a nil data object will remove any existing value
 * from the cache.<br />
 * You may use this method to control what data is used for specified
 * file names.
 */
+ (void) setData: (NSData*)data forTLSFile: (NSString*)fileName;

@end

/* This class provides the current autogenerated Diffie Hellman parameters
 * for server negotiation and/or parameters laoded from file.
 */
GS_EXPORT_CLASS
@interface      GSTLSDHParams : GSTLSObject
{
  NSTimeInterval        when;
  NSString              *path;
  gnutls_dh_params_t    params;
}

/* Returns the most recently generated key ... if there is none this calls
 * +generate to create one.  Once a key has been generated, replacements
 * are periodically generated in a separate thread.
 */
+ (GSTLSDHParams*) current;

/* Generate key ... really slow.
 */
+ (void) generate;

/* Return params loaded from a file.
 */
+ (GSTLSDHParams*) paramsFromFile: (NSString*)f;

- (gnutls_dh_params_t) params;
@end

/* Manage certificate lists (for servers and clients) and also provide
 * DH params.
 */
GS_EXPORT_CLASS
@interface      GSTLSCertificateList : GSTLSObject
{
  NSTimeInterval        when;
  NSString              *path;
  gnutls_x509_crt_t     *crts;
  unsigned int          count;
}
+ (GSTLSCertificateList*) listFromFile: (NSString*)f;

/* Return the list of x509 certificates.
 */
- (gnutls_x509_crt_t*) certificateList;

/* Return number of certificates in list.
 */
- (unsigned int) count;

/* Return the earliest expiry date of any certificate in the list.
 */
- (NSDate*) expiresAt;

/* Return the date when a certificate in the list expires.
 */
- (NSDate*) expiresAt: (unsigned int)index;
@end

/* This encapsulates private keys used to unlock certificates
 */
GS_EXPORT_CLASS
@interface      GSTLSPrivateKey : GSTLSObject
{
  NSTimeInterval        when;
  NSString              *path;
  NSString              *password;
  gnutls_x509_privkey_t key;
}
+ (GSTLSPrivateKey*) keyFromFile: (NSString*)f withPassword: (NSString*)p;
- (gnutls_x509_privkey_t) key;
@end

/* This encapsulates a credentials setup for a sessions
 */
GS_EXPORT_CLASS
@interface      GSTLSCredentials : GSTLSObject
{
  NSTimeInterval                        when;
  NSString                              *name;
  GSTLSPrivateKey                       *key;
  GSTLSCertificateList                  *list;
  GSTLSDHParams                         *dhParams;
  BOOL                                  trust;
  BOOL                                  freeCred;
  gnutls_certificate_credentials_t      certcred;
}
+ (GSTLSCredentials*) credentialsFromCAFile: (NSString*)ca
                              defaultCAFile: (NSString*)dca
                                 revokeFile: (NSString*)rv
                          defaultRevokeFile: (NSString*)drv
                            certificateFile: (NSString*)cf
                         certificateKeyFile: (NSString*)ck
                     certificateKeyPassword: (NSString*)cp
                                   asClient: (BOOL)client
                                      debug: (BOOL)debug;
+ (GSTLSCredentials*) selfSigned: (BOOL)debug;
- (gnutls_certificate_credentials_t) credentials;
- (GSTLSPrivateKey*) key;
- (GSTLSCertificateList*) list;
- (BOOL) trust;
@end


/* Declare a pointer to a function to be used for I/O
 */
typedef ssize_t (*GSTLSIOR)(gnutls_transport_ptr_t, void *, size_t);
typedef ssize_t (*GSTLSIOW)(gnutls_transport_ptr_t, const void *, size_t);

/* This class encapsulates a session to a remote system.
 * Sessions are created with a direction and an options dictionary,
 * defining how they will operate.  The handle, pushFunc and pullFunc
 * provide the I/O mechanism.
 */
GS_EXPORT_CLASS
@interface      GSTLSSession : GSTLSObject
{
  NSDictionary                          *opts;
  GSTLSCredentials                      *credentials;
  NSString                              *problem;
  NSString                              *issuer;
  NSString                              *owner;
  BOOL                                  outgoing;
  BOOL                                  active;
  BOOL                                  handshake;
  BOOL                                  setup;
  BOOL                                  debug;
  NSTimeInterval                        created;
  void                                  *handle;
@public
  gnutls_session_t                      session;
}
+ (GSTLSSession*) sessionWithOptions: (NSDictionary*)options
                           direction: (BOOL)isOutgoing
                           transport: (void*)ioHandle
                                push: (GSTLSIOW)pushFunc
                                pull: (GSTLSIOR)pullFunc;

- (id) initWithOptions: (NSDictionary*)options
             direction: (BOOL)isOutgoing
             transport: (void*)ioHandle
                  push: (GSTLSIOW)pushFunc
                  pull: (GSTLSIOR)pullFunc;

/* Return YES if the session is active (handshake has succeeded and the
 * session has not been disconnected), NO otherwise.
 */
- (BOOL) active;

/** Returns the age of this instance (how long since it was created).
 */
- (NSTimeInterval) age;

/* Returns the credentials object ofr this session.
 */
- (GSTLSCredentials*) credentials;

/* Return the current debug mode.
 */
- (BOOL) debug;

/* Disconnects and closes down the session.<br />
 * The reusable flag specifies whether we intend to reuse the underlying
 * connection.<br />
 * Returns YES on success, NO if the shutdown did not complete cleanly
 * and the underlying connection cannot be reused.
 */
- (BOOL) disconnect: (BOOL)reusable;

/* Try to complete a handshake ... return YES if complete, NO if we need
 * to try again (would have to wait for the remote end).<br />
 */
- (BOOL) handshake;

/** If the session verified a certificate from the remote end, returns the
 * name of the certificate issuer in the form "C=xxxx,O=yyyy,CN=zzzz" as
 * described in RFC4514.  Otherwise returns nil.
 */
- (NSString*) issuer;

/** If the session verified a certificate from the remote end, returns the
 * name of the certificate owner in the form "C=xxxx,O=yyyy,CN=zzzz" as
 * described in RFC4514.  Otherwise returns nil.
 */
- (NSString*) owner;

/** Returns the number of bytes of data available to be read from the TLS
 * buffers (using the -read:length: method).  If this returns zero the TLS
 * software needs to perform a network read before any more data can be
 * returned.
 */
- (size_t) pending;

/* After a failed handshake, this should contain a description of the
 * failure reason.
 */
- (NSString*) problem;

/* Read data from the session.
 */
- (NSInteger) read: (void*)buf length: (NSUInteger)len;

/** Get a report of the SSL/TLS status of the current session.
 */
- (NSString*) sessionInfo;

/* Write data to the session.
 */
- (NSInteger) write: (const void*)buf length: (NSUInteger)len;

/* For internal use to verify the remote system's certificate.
 * Returns 0 on success, negative on failure.
 */
- (int) verify;
@end

#endif

