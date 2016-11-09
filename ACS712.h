#ifndef __ACS712_H__
#define __ACS712_H__

class ACS712 {
public:
	enum Type {
		A5,
		A20,
		A30
	};
	ACS712(Type type, int pin);
	virtual ~ACS712();
	void begin();
	void zero();
	double measure(unsigned int samples) const;

private:
	double sample(unsigned int samples) const;

	Type _type;
	int _pin;

	double _zeroAdc;
	double _mvPerAmp;
};

#endif
