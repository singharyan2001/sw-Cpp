#ifndef ISENSOR_H
#define ISENSOR_H

enum Status{
    SENSOR_OK,
    SENSOR_ERROR
};

class ISensor {
public:
    virtual bool init() = 0;
    virtual Status getStatus() const = 0;   // virtual getter
    virtual float readValue() = 0;

    virtual ~ISensor() = default;   // Safe!
};

#endif // ISENSOR_H