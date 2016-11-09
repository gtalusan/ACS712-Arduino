#include "ACS712.h"

ACS712 acs712(ACS712::Type::A5, A1);

void setup()
{
	Serial.begin(115200);
	acs712.begin();
}

void loop(void)
{
	double amps = acs712.measure(1000);
	Serial.println(amps, 4);
	delay(1000);
}
