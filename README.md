# IoT-ML Weather & Air Quality Monitoring System

An embedded IoT-based weather and air quality monitoring system developed on the STM32 platform. The project follows a modular software architecture and integrates environmental monitoring with a lightweight Machine Learning prediction model for PM2.5 forecasting.

---

# Overview

This project was developed as a team project for an Embedded Systems course.

The system periodically acquires environmental data, processes the measurements using digital filtering techniques, calculates the Air Quality Index (AQI), evaluates alarm conditions, logs system information through UART, and predicts future PM2.5 values using a Machine Learning model trained with Scikit-learn.

The project was implemented in two development phases.

---

# Key Features

| Feature | Status |
|----------|--------|
| Modular Software Architecture | ✅ |
| Cooperative Scheduler | ✅ |
| Mock Sensor Subsystem | ✅ |
| Digital PM2.5 Filtering | ✅ |
| AQI Calculation | ✅ |
| Air Quality Classification | ✅ |
| Alarm Management | ✅ |
| UART Logger | ✅ |
| AQI Trend Detection | ✅ |
| PM2.5 Statistics | ✅ |
| Machine Learning Prediction | ✅ |
| Embedded Predictor Module | ✅ |

---

# Features

## Phase 1

- Modular embedded software architecture
- STM32 HAL-based implementation
- Cooperative scheduler
- Mock sensor subsystem
- PM2.5 exponential moving average (EMA) filter
- AQI calculation
- Air quality classification
- Alarm management
- UART logging
- Configurable system parameters

---

## Phase 2

- Machine Learning prediction module
- Enhanced filtering algorithms
- Moving average filtering
- PM2.5 statistics
- AQI statistics
- AQI trend detection
- Advanced alarm levels
- Alarm hysteresis
- Alarm reason detection
- Future PM2.5 estimation
- Embedded predictor interface
- Improved logging output

---

# Software Architecture

```text
                 +----------------+
                 |  Mock Sensors  |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 |    Filter      |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 |      AQI       |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 | Predictor (ML) |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 |     Alarm      |
                 +-------+--------+
                         |
                         v
                 +----------------+
                 |     Logger     |
                 +----------------+
```

The cooperative scheduler periodically executes each software module according to predefined timing intervals.

---

# Project Structure

```text
Core/
│
├── Inc/
│   ├── alarm.h
│   ├── aqi.h
│   ├── filter.h
│   ├── predictor.h
│   ├── logger.h
│   ├── scheduler.h
│   ├── system_data.h
│   ├── mock_sensor.h
│   └── app_config.h
│
├── Src/
    ├── alarm.c
    ├── aqi.c
    ├── filter.c
    ├── predictor.c
    ├── logger.c
    ├── scheduler.c
    ├── system_data.c
    ├── mock_sensor.c
    └── main.c

```

---

# Machine Learning Module

The prediction model was developed separately using Python and Scikit-learn.

### Workflow

1. Dataset preprocessing
2. Feature engineering
3. Linear Regression model training
4. Model evaluation
5. Weight extraction
6. Manual deployment of learned coefficients into the embedded firmware

### Input Features

- Current PM2.5
- Previous PM2.5 (t−1)
- Previous PM2.5 (t−2)
- Previous PM2.5 (t−3)
- Temperature
- Pressure

### Output

- Predicted future PM2.5 concentration

The embedded firmware performs **inference only**, while model training is completed offline.

---

# Dataset

The Machine Learning model was trained using the **Beijing PM2.5 Data Set**, a publicly available environmental dataset.

Selected features:

- PM2.5
- PM2.5(t−1)
- PM2.5(t−2)
- PM2.5(t−3)
- Temperature
- Pressure

Feature engineering was performed to generate time-lag variables used for future PM2.5 prediction.

---

# Machine Learning Performance

Model:

- Linear Regression (Scikit-learn)

Evaluation Metrics:

- MAE: 12.943626500800077
- RMSE: 23.518444415363035
- R² Score: 0.9339776108186009

---

# Technologies

- C
- STM32 HAL
- STM32CubeMX
- Keil MDK (uVision)
- Python
- Scikit-learn
- Jupyter Notebook
- Git
- GitHub

---

# Scheduler Flow

```text
while(1)
{
    Scheduler_Run();
}

Scheduler_Run()

    ├── MockSensor_Update()
    ├── Filter_Update()
    ├── AQI_Update()
    ├── Predictor_Update()
    ├── Alarm_Update()
    └── Logger_Report()
```

---

# Air Quality Processing Pipeline

The system processes environmental data through multiple software layers:

1. Raw sensor acquisition
2. Digital signal filtering
3. PM2.5 smoothing
4. AQI computation
5. Air quality classification
6. Machine Learning prediction
7. Alarm generation
8. UART reporting

---

# Build Instructions

1. Open the project using **STM32CubeMX**.
2. Generate the project for **Keil MDK**.
3. Open the generated project in **Keil uVision**.
4. Build the firmware.
5. Flash the firmware to the STM32 development board.
6. Open a serial terminal configured at **115200 baud** to monitor the system output.

---

# Team Contributions

## Behnam Gholam Zadeh Nahary — Team Leader

- Overall software architecture
- Project planning and coordination
- Module integration
- Scheduler design
- System data management
- Machine Learning pipeline
- Embedded predictor module
- GitHub repository management
- Code review and integration
- Final testing and debugging
- Technical documentation

## Yashar Salmani

Data Filtering Module:
- Designed a data filtering system using EMA and Moving Average
- Added adaptive smoothing, spike detection, and outlier rejection
- Tracked min/max values, filtered output, data status, and rejected samples
Fault Detection Module:
- Implemented fault detection for abnormal values, invalid CO₂ readings, and sensor errors
- Integrated fault flags for real-time monitoring
PM2.5 Sensor Module:
- Configured STM32 peripherals (ADC1, USART1, GPIO, and clock) in STM32CubeMX
- Developed firmware for PM2.5 data acquisition, calculation, and UART transmission
- Simulated and validated the system in Proteus

## Sama Nouri

- AQI calculation module
- AQI statistics
- AQI trend detection
- ADC-based sensor integration
- MQ2 sensor simulation in Proteus
- LED-based air quality indication
- History module with circular buffer
- Historical sensor data storage

## Helia Salmasi

- Implement multie-level alarm system (Normal, Warning, Critical)
- Add alarm system management
- Implement hysteresis to prevent alarm oscillation
- Count the total number of alarm events
- Store the reason for each alarm trigger
- Update alarm status based on AQI tresholds
- Handle critical alarm conditions
- Reset alarm state  when air quality returns to safe range
- Keep the existing public API unchanged
- Test alarm behavior under different AQI scenarios
- Temperature and humidity sensor design

## Ali Arshia Maleki

- UART logging system enhancement
- System status reporting
- Alarm monitoring and reporting
- Fault monitoring integration
- History statistics reporting
- AQI forecast integration
- Air quality trend reporting
- BMP280 pressure sensor evaluation
- I2C protocol study
- Pressure sensor simulation (Mock)
- Pressure data generation
- Module testing and build verification

## Member 6

- Sensor subsystem
- Sensor abstraction layer
- Mock sensor implementation
- Future real sensor integration

---

# Future Improvements

- Integration with real environmental sensors
- SD Card data logging
- Interactive CLI interface
- OLED/LCD display support
- MQTT communication
- Wi-Fi connectivity
- Cloud dashboard
- TinyML deployment
- TensorFlow Lite for Microcontrollers
- Edge AI optimization

---

# Build Environment

- STM32CubeMX
- Keil MDK
- ARM Compiler
- STM32 HAL Drivers

---

# License

This project was developed for educational purposes as part of an Embedded Systems course.
