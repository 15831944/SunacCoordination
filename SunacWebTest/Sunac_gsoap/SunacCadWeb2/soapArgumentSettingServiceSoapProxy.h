/* soapArgumentSettingServiceSoapProxy.h
   Generated by gSOAP 2.8.84 for .\SunacCADService2.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapArgumentSettingServiceSoapProxy_H
#define soapArgumentSettingServiceSoapProxy_H
#include "soapH.h"

    class SOAP_CMAC ArgumentSettingServiceSoapProxy : public soap {
      public:
        /// Endpoint URL of service 'ArgumentSettingServiceSoapProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in .\SunacCADService2.h, if any
        /// Construct a proxy with new managing context
        ArgumentSettingServiceSoapProxy();
        /// Copy constructor
        ArgumentSettingServiceSoapProxy(const ArgumentSettingServiceSoapProxy& rhs);
        /// Construct proxy given a managing context
        ArgumentSettingServiceSoapProxy(const struct soap&);
        /// Construct proxy given a managing context and endpoint URL
        ArgumentSettingServiceSoapProxy(const struct soap&, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        ArgumentSettingServiceSoapProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        ArgumentSettingServiceSoapProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        ArgumentSettingServiceSoapProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        ArgumentSettingServiceSoapProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context
        virtual ~ArgumentSettingServiceSoapProxy();
        /// Initializer used by constructors
        virtual void ArgumentSettingServiceSoapProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual ArgumentSettingServiceSoapProxy *copy();
        /// Copy assignment
        ArgumentSettingServiceSoapProxy& operator=(const ArgumentSettingServiceSoapProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'StandardDesignAttribute' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int StandardDesignAttribute(_ns1__StandardDesignAttribute *ns1__StandardDesignAttribute, _ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse) { return this->StandardDesignAttribute(NULL, NULL, ns1__StandardDesignAttribute, ns1__StandardDesignAttributeResponse); }
        /// Web service synchronous operation 'StandardDesignAttribute' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int StandardDesignAttribute(const char *soap_endpoint_url, const char *soap_action, _ns1__StandardDesignAttribute *ns1__StandardDesignAttribute, _ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse) { return this->send_StandardDesignAttribute(soap_endpoint_url, soap_action, ns1__StandardDesignAttribute) || this->recv_StandardDesignAttribute(ns1__StandardDesignAttributeResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_StandardDesignAttribute' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_StandardDesignAttribute(const char *soap_endpoint_url, const char *soap_action, _ns1__StandardDesignAttribute *ns1__StandardDesignAttribute);
        /// Web service asynchronous operation 'recv_StandardDesignAttribute' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_StandardDesignAttribute(_ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse);
        //
        /// Web service synchronous operation 'GetAllWindows' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAllWindows(_ns1__GetAllWindows *ns1__GetAllWindows, _ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse) { return this->GetAllWindows(NULL, NULL, ns1__GetAllWindows, ns1__GetAllWindowsResponse); }
        /// Web service synchronous operation 'GetAllWindows' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAllWindows(const char *soap_endpoint_url, const char *soap_action, _ns1__GetAllWindows *ns1__GetAllWindows, _ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse) { return this->send_GetAllWindows(soap_endpoint_url, soap_action, ns1__GetAllWindows) || this->recv_GetAllWindows(ns1__GetAllWindowsResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAllWindows' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAllWindows(const char *soap_endpoint_url, const char *soap_action, _ns1__GetAllWindows *ns1__GetAllWindows);
        /// Web service asynchronous operation 'recv_GetAllWindows' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAllWindows(_ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse);
        //
        /// Web service synchronous operation 'GetWindows' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetWindows(_ns1__GetWindows *ns1__GetWindows, _ns1__GetWindowsResponse &ns1__GetWindowsResponse) { return this->GetWindows(NULL, NULL, ns1__GetWindows, ns1__GetWindowsResponse); }
        /// Web service synchronous operation 'GetWindows' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetWindows(const char *soap_endpoint_url, const char *soap_action, _ns1__GetWindows *ns1__GetWindows, _ns1__GetWindowsResponse &ns1__GetWindowsResponse) { return this->send_GetWindows(soap_endpoint_url, soap_action, ns1__GetWindows) || this->recv_GetWindows(ns1__GetWindowsResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetWindows' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetWindows(const char *soap_endpoint_url, const char *soap_action, _ns1__GetWindows *ns1__GetWindows);
        /// Web service asynchronous operation 'recv_GetWindows' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetWindows(_ns1__GetWindowsResponse &ns1__GetWindowsResponse);
        //
        /// Web service synchronous operation 'StandardDesignAttribute' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int StandardDesignAttribute_(_ns1__StandardDesignAttribute *ns1__StandardDesignAttribute, _ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse) { return this->StandardDesignAttribute_(NULL, NULL, ns1__StandardDesignAttribute, ns1__StandardDesignAttributeResponse); }
        /// Web service synchronous operation 'StandardDesignAttribute' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int StandardDesignAttribute_(const char *soap_endpoint_url, const char *soap_action, _ns1__StandardDesignAttribute *ns1__StandardDesignAttribute, _ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse) { return this->send_StandardDesignAttribute_(soap_endpoint_url, soap_action, ns1__StandardDesignAttribute) || this->recv_StandardDesignAttribute_(ns1__StandardDesignAttributeResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_StandardDesignAttribute' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_StandardDesignAttribute_(const char *soap_endpoint_url, const char *soap_action, _ns1__StandardDesignAttribute *ns1__StandardDesignAttribute);
        /// Web service asynchronous operation 'recv_StandardDesignAttribute' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_StandardDesignAttribute_(_ns1__StandardDesignAttributeResponse &ns1__StandardDesignAttributeResponse);
        //
        /// Web service synchronous operation 'GetAllWindows' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetAllWindows_(_ns1__GetAllWindows *ns1__GetAllWindows, _ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse) { return this->GetAllWindows_(NULL, NULL, ns1__GetAllWindows, ns1__GetAllWindowsResponse); }
        /// Web service synchronous operation 'GetAllWindows' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetAllWindows_(const char *soap_endpoint_url, const char *soap_action, _ns1__GetAllWindows *ns1__GetAllWindows, _ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse) { return this->send_GetAllWindows_(soap_endpoint_url, soap_action, ns1__GetAllWindows) || this->recv_GetAllWindows_(ns1__GetAllWindowsResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetAllWindows' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetAllWindows_(const char *soap_endpoint_url, const char *soap_action, _ns1__GetAllWindows *ns1__GetAllWindows);
        /// Web service asynchronous operation 'recv_GetAllWindows' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetAllWindows_(_ns1__GetAllWindowsResponse &ns1__GetAllWindowsResponse);
        //
        /// Web service synchronous operation 'GetWindows' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetWindows_(_ns1__GetWindows *ns1__GetWindows, _ns1__GetWindowsResponse &ns1__GetWindowsResponse) { return this->GetWindows_(NULL, NULL, ns1__GetWindows, ns1__GetWindowsResponse); }
        /// Web service synchronous operation 'GetWindows' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetWindows_(const char *soap_endpoint_url, const char *soap_action, _ns1__GetWindows *ns1__GetWindows, _ns1__GetWindowsResponse &ns1__GetWindowsResponse) { return this->send_GetWindows_(soap_endpoint_url, soap_action, ns1__GetWindows) || this->recv_GetWindows_(ns1__GetWindowsResponse) ? this->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetWindows' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetWindows_(const char *soap_endpoint_url, const char *soap_action, _ns1__GetWindows *ns1__GetWindows);
        /// Web service asynchronous operation 'recv_GetWindows' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetWindows_(_ns1__GetWindowsResponse &ns1__GetWindowsResponse);
    };
#endif
