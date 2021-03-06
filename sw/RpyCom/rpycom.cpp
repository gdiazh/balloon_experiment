/**
 * @brief Simple Raspberry Pi communication Library
 */

/*Author: Matías Vidal*/

/*Requiered Libraries*/
#include "rpycom.h"

void RPYCOM::updateBeacon(double dataD[], float dataF[], uint8_t dataU8[], uint32_t dataU32)
{   
    beacon_tx_.Temp1 = (float) dataD[0];
    beacon_tx_.Pressure = (float) dataD[1];
    beacon_tx_.Alt = (float) dataD[2];
    beacon_tx_.Temp2 = dataF[0];
    beacon_tx_.Humidity = dataF[1];
    beacon_tx_.Temp3 = dataF[2];
    beacon_tx_.IMU1 = dataF[3];
    beacon_tx_.IMU2 = dataF[4];
    beacon_tx_.IMU3 = dataF[5];
    beacon_tx_.GPS_Lat = (float) dataD[3];
    beacon_tx_.GPS_Lng = (float) dataD[4];
    beacon_tx_.GPS_Alt = (float) dataD[5];
    beacon_tx_.GPS_Crse = (float) dataD[6];
    beacon_tx_.GPS_Speed = (float) dataD[7];
    beacon_tx_.GPS_HH = dataU8[0];
    beacon_tx_.GPS_MM = dataU8[1];
    beacon_tx_.GPS_SS = dataU8[2];
    beacon_tx_.GPS_validity = dataU8[3];
    beacon_tx_.GPS_Sat = dataU32;
}

void RPYCOM::BeaconTest()
{   
    beacon_tx_.Temp1 = 123.45;
    beacon_tx_.Pressure = 956.76;
    beacon_tx_.Alt = -75.01;
    beacon_tx_.Temp2 = 123.45;
    beacon_tx_.Humidity = 956.76;
    beacon_tx_.Temp3 = 956.76;
    beacon_tx_.IMU1 = -75.01;
    beacon_tx_.IMU2 = -75.01;
    beacon_tx_.IMU3 = -75.01;
    beacon_tx_.GPS_Lat = 98.3;
    beacon_tx_.GPS_Lng = 21.7;
    beacon_tx_.GPS_Alt = 87.80;
    beacon_tx_.GPS_Crse = 101.71;
    beacon_tx_.GPS_Speed = 43.87;
    beacon_tx_.GPS_HH = 241;
    beacon_tx_.GPS_MM = 32;
    beacon_tx_.GPS_SS = 85;
    beacon_tx_.GPS_validity = 255;
    beacon_tx_.GPS_Sat = 4294967295;
}

void RPYCOM::getData()
{
	// uint8_t frame[PACKET_SZ];
	memset(frame_rpy2base_, 0, PACKET_SZ);
	uint8_t readed_bytes = hw_port_->readBytes((char*)&frame_rpy2base_, PACKET_SZ);
	if (readed_bytes == PACKET_SZ)
	{
	    cmd_.node = frame_rpy2base_[0];
	    cmd_.port = frame_rpy2base_[1];
	    Serial.print("node:");Serial.println(cmd_.node);
	    Serial.print("port:");Serial.println(cmd_.port);
	}
	else
	{
		cmd_.node = 0;
	    cmd_.port = 0;
	}
	if (readed_bytes==PACKET_SZ)
	{
		for (int i = 0; i < PACKET_SZ-1; i++)
		{
			Serial.print(frame_rpy2base_[i]);Serial.print(",");
		}
		Serial.println("end");
	}
	else
	{
		Serial.print("readed_bytes:");Serial.println(readed_bytes);
	}

}

void RPYCOM::resetStrutures()
{
	cmd_.node = 0;
	cmd_.port = 0;
	beacon_tx_.Temp1 = 0;
    beacon_tx_.Pressure = 0;
    beacon_tx_.Alt = 0;
    beacon_tx_.Temp2 = 0;
    beacon_tx_.Humidity = 0;
    beacon_tx_.Temp3 = 0;
    beacon_tx_.IMU1 = 0;
    beacon_tx_.IMU2 = 0;
    beacon_tx_.IMU3 = 0;
    beacon_tx_.GPS_Lat = 0;
    beacon_tx_.GPS_Lng = 0;
    beacon_tx_.GPS_Alt = 0;
    beacon_tx_.GPS_Crse = 0;
    beacon_tx_.GPS_Speed = 0;
    beacon_tx_.GPS_HH = 0;
    beacon_tx_.GPS_MM = 0;
    beacon_tx_.GPS_SS = 0;
    beacon_tx_.GPS_validity = 0;
    beacon_tx_.GPS_Sat = 0;
	memset(data_, 0, DATA_SZ);
}

void RPYCOM::sendData()
{
	uint8_t padd_sz = PACKET_SZ-CMD_SZ-beacon_tx_size_;
	uint8_t padding[padd_sz];
	memset(padding, 0, padd_sz);

	hw_port_->write(node_);
	hw_port_->write(port_);
	hw_port_->write((uint8_t *)&beacon_tx_, beacon_tx_size_);
	hw_port_->write(padding, padd_sz);
}

void RPYCOM::send_msg(char *msg)
{   
	// char *msg = "reset";
    uint8_t padd_sz = PACKET_SZ-CMD_SZ-strlen(msg);
	uint8_t padding[padd_sz];
	memset(padding, 0, padd_sz);

	hw_port_->write(1);
	hw_port_->write(11);
	hw_port_->write(msg, strlen(msg));
	hw_port_->write(padding, padd_sz);
}