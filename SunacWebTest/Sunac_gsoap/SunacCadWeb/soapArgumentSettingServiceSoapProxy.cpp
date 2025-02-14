/* soapArgumentSettingServiceSoapProxy.cpp
   Generated by gSOAP 2.8.84 for .\SunacCADService.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapArgumentSettingServiceSoapProxy.h"

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy() : soap(SOAP_IO_DEFAULT)
{	ArgumentSettingServiceSoapProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(const ArgumentSettingServiceSoapProxy& rhs)
{	soap_copy_context(this, &rhs);
	this->soap_endpoint = rhs.soap_endpoint;
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(const struct soap &_soap) : soap(_soap)
{ }

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(const struct soap &_soap, const char *soap_endpoint_url) : soap(_soap)
{
	soap_endpoint = soap_endpoint_url;
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(const char *soap_endpoint_url) : soap(SOAP_IO_DEFAULT)
{	ArgumentSettingServiceSoapProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = soap_endpoint_url;
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(soap_mode iomode) : soap(iomode)
{	ArgumentSettingServiceSoapProxy_init(iomode, iomode);
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(const char *soap_endpoint_url, soap_mode iomode) : soap(iomode)
{	ArgumentSettingServiceSoapProxy_init(iomode, iomode);
	soap_endpoint = soap_endpoint_url;
}

ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy(soap_mode imode, soap_mode omode) : soap(imode, omode)
{	ArgumentSettingServiceSoapProxy_init(imode, omode);
}

ArgumentSettingServiceSoapProxy::~ArgumentSettingServiceSoapProxy()
{
	this->destroy();
}

void ArgumentSettingServiceSoapProxy::ArgumentSettingServiceSoapProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL },
        { "SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "AS2", "http://tempuri.org/ArgumentSettingServiceSoap", NULL, NULL },
        { "AS", "http://tempuri.org/", NULL, NULL },
        { "AS3", "http://tempuri.org/ArgumentSettingServiceSoap12", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this, namespaces);
}

ArgumentSettingServiceSoapProxy *ArgumentSettingServiceSoapProxy::copy()
{	ArgumentSettingServiceSoapProxy *dup = SOAP_NEW_UNMANAGED(ArgumentSettingServiceSoapProxy(*(struct soap*)this));
	return dup;
}

ArgumentSettingServiceSoapProxy& ArgumentSettingServiceSoapProxy::operator=(const ArgumentSettingServiceSoapProxy& rhs)
{	soap_done(this);
	soap_copy_context(this, &rhs);
	this->soap_endpoint = rhs.soap_endpoint;
	return *this;
}

void ArgumentSettingServiceSoapProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void ArgumentSettingServiceSoapProxy::reset()
{	this->destroy();
	soap_done(this);
	soap_initialize(this);
	ArgumentSettingServiceSoapProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void ArgumentSettingServiceSoapProxy::soap_noheader()
{	this->header = NULL;
}

::SOAP_ENV__Header *ArgumentSettingServiceSoapProxy::soap_header()
{	return this->header;
}

::SOAP_ENV__Fault *ArgumentSettingServiceSoapProxy::soap_fault()
{	return this->fault;
}

const char *ArgumentSettingServiceSoapProxy::soap_fault_subcode()
{	return ::soap_fault_subcode(this);
}

const char *ArgumentSettingServiceSoapProxy::soap_fault_string()
{	return ::soap_fault_string(this);
}

const char *ArgumentSettingServiceSoapProxy::soap_fault_detail()
{	return ::soap_fault_detail(this);
}

int ArgumentSettingServiceSoapProxy::soap_close_socket()
{	return ::soap_closesock(this);
}

int ArgumentSettingServiceSoapProxy::soap_force_close_socket()
{	return ::soap_force_closesock(this);
}

void ArgumentSettingServiceSoapProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void ArgumentSettingServiceSoapProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *ArgumentSettingServiceSoapProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int ArgumentSettingServiceSoapProxy::send_StandardDesignAttribute(const char *soap_endpoint_url, const char *soap_action, _AS__StandardDesignAttribute *AS__StandardDesignAttribute)
{
	struct soap *soap = this;
	struct __AS2__StandardDesignAttribute soap_tmp___AS2__StandardDesignAttribute;
	if (soap_endpoint_url != NULL)
		soap_endpoint = soap_endpoint_url;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://10.4.64.91/SunacCADService.asmx";
	if (soap_action == NULL)
		soap_action = "http://tempuri.org/StandardDesignAttribute";
	soap_tmp___AS2__StandardDesignAttribute.AS__StandardDesignAttribute = AS__StandardDesignAttribute;
	soap_begin(soap);
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize___AS2__StandardDesignAttribute(soap, &soap_tmp___AS2__StandardDesignAttribute);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___AS2__StandardDesignAttribute(soap, &soap_tmp___AS2__StandardDesignAttribute, "-AS2:StandardDesignAttribute", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___AS2__StandardDesignAttribute(soap, &soap_tmp___AS2__StandardDesignAttribute, "-AS2:StandardDesignAttribute", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int ArgumentSettingServiceSoapProxy::recv_StandardDesignAttribute(_AS__StandardDesignAttributeResponse &AS__StandardDesignAttributeResponse)
{
	struct soap *soap = this;
	AS__StandardDesignAttributeResponse.soap_default(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	AS__StandardDesignAttributeResponse.soap_get(soap, "AS:StandardDesignAttributeResponse", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
/* End of client proxy code */
