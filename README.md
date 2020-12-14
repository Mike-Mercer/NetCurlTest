This is another test for issue described here https://github.com/Mike-Mercer/NetworkTest/blob/master/NetTest.cpp

We used libcurl compiled with OpenSSL to discover more details on the REST FAPI  disconnections.
`vcpkg.exe  install curl[core,openssl]:x64-windows`

This is one of such cases: the connection was suddenly reseted inside of SSL_read(); libcurl does not report this as an error but just retries new request, 
so If you don't expect such disconnections, you will never notice them 

```
* Connection #1 to host fapi.binance.com left intact
* Found bundle for host fapi.binance.com: 0x18612981ab8 [serially]
* Re-using existing connection! (#1) with host fapi.binance.com
* Connected to fapi.binance.com (52.84.150.39) port 443 (#1)
> GET /fapi/v1/ticker/bookTicker HTTP/1.1

Host: fapi.binance.com
Accept: */*
Accept-Encoding: gzip
Connection: Keep-Alive

* OpenSSL SSL_read: Connection was reset, errno 10054
* Connection died, retrying a fresh connect
* Closing connection 1
* Issue another request to this URL: 'https://fapi.binance.com/fapi/v1/ticker/bookTicker'
*   Trying 52.84.150.39:443...
* TCP_NODELAY set
* Connected to fapi.binance.com (52.84.150.39) port 443 (#2)
* ALPN, offering http/1.1
* SSL connection using TLSv1.3 / TLS_AES_128_GCM_SHA256
* ALPN, server accepted to use http/1.1
* Server certificate:
*  subject: C=KY; L=George Town; O=Binance Holdings Limited; OU=IT; CN=*.binance.com
*  start date: Jan  7 00:00:00 2020 GMT
*  expire date: Apr  7 12:00:00 2022 GMT
*  subjectAltName: host "fapi.binance.com" matched cert's "*.binance.com"
*  issuer: C=US; O=DigiCert Inc; OU=www.digicert.com; CN=GeoTrust RSA CA 2018
*  SSL certificate verify result: unable to get local issuer certificate (20), continuing anyway.
> GET /fapi/v1/ticker/bookTicker HTTP/1.1

Host: fapi.binance.com
```
