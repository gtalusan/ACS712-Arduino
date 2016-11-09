#include "ACS712.h"
#include "Arduino.h"

static long readVcc()
{
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
	delay(2);
	ADCSRA |= _BV(ADSC);
	while (bit_is_set(ADCSRA, ADSC));

	uint8_t low  = ADCL;
	uint8_t high = ADCH;

	long result = (high << 8) | low;

	result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	return result;
}

ACS712::ACS712(ACS712::Type type, int pin) : _type(type), _pin(pin), _zeroAdc(512.0)
{
	if (type == A30) {
		_mvPerAmp = 66.0;
	}
	else if (type == A20) {
		_mvPerAmp = 100.0;
	}
	else if (type == A5) {
		_mvPerAmp = 185.0;
	}
}

ACS712::~ACS712()
{
}

void ACS712::begin()
{
	pinMode(_pin, INPUT);
}

double ACS712::sample(unsigned int samples) const
{
	double sum = 0;
	for (unsigned int i = 0; i < samples; i++) {
		double a = analogRead(_pin);
		sum += a * a;
	}
	double rms = sqrt(sum / 1000);
	return rms;
}

void ACS712::zero()
{
	_zeroAdc = sample(1000);
}

double ACS712::measure(unsigned int samples) const
{
	long mvRef = readVcc();
	double adc = sample(samples);
	Serial.println(adc);
	double mv = ((adc - _zeroAdc) / 1023.0) * mvRef;
	mv /= _mvPerAmp;
	return mv;
}

