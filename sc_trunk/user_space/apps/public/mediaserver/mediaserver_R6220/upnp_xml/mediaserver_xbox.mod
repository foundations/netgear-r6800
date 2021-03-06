<?xml version="1.0"?>
<root xmlns="urn:schemas-upnp-org:device-1-0">
	<specVersion>
		<major>1</major>
		<minor>0</minor>
	</specVersion>
	<device>
		<deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>
		<friendlyName>@FRIENDLY_NAME#:Media Server</friendlyName>
		<manufacturer>@MANUFACTURER#</manufacturer>
		<manufacturerURL>@MANUFACTURER_URL#</manufacturerURL>
		<modelDescription>@MODEL_DESCRIPTION#</modelDescription>
		<modelName>Windows Media Connect Compatible(@MODEL_NAME#)</modelName>
		<modelNumber>3.0</modelNumber>
		<serialNumber>@SERIAL_NUMBER#</serialNumber>
		<UDN>uuid:@UUID#</UDN>
		<serviceList>
			<service>
				<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType>
				<serviceId>urn:upnp-org:serviceId:ConnectionManager</serviceId>
				<SCPDURL>/connection.xml</SCPDURL>
				<eventSubURL>/ConnectionManager/Event</eventSubURL>
				<controlURL>/ConnectionManager/Control</controlURL>
			</service>
			<service>
				<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>
				<serviceId>urn:upnp-org:serviceId:ContentDirectory</serviceId>
				<SCPDURL>/content.xml</SCPDURL>
				<eventSubURL>/ContentDirectory/Event</eventSubURL>
				<controlURL>/ContentDirectory/Control</controlURL>
			</service>
			<service>
				<serviceType>urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1</serviceType>
				<serviceId>urn:microsoft.com:serviceId:X_MS_MediaReceiverRegistrar</serviceId>
				<SCPDURL>/mr_reg.xml</SCPDURL>
				<eventSubURL>MediaReceiverRegistrar/Event</eventSubURL>
				<controlURL>MediaReceiverRegistrar/Control</controlURL>
			</service>
		</serviceList>
		 <dlna:X_DLNADOC xmlns:dlna="urn:schemas-dlna-org:device-1-0">DMS-1.50</dlna:X_DLNADOC> 
	</device>
</root>
