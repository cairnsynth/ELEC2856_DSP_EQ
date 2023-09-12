# ELEC2856 Digital EQ
- Realtime audio equaliser developed on the Nucleo-L476 development board (STM32)
- Implements 1st and 2nd order IIR filters
- Features adjustable gain and cut-off for each frequency band
- Aimed to emulate the level of control found on DJ mixers
---
## Design
- A system diagram is shown below

![Block diagram drawio (1)](https://github.com/luke1241/ELEC2856_DSP_EQ/assets/95569413/53bb358a-eac7-48be-97f7-48cd85944865)

### Software
- Class diagram of the EQ software is shown below

![class diagram drawio](https://github.com/luke1241/ELEC2856_DSP_EQ/assets/95569413/771b4179-7d31-4471-a74e-e5ba4147a8cf)

### Hardware
- User input implemented with potentiometers
- Additional signal conditioning was implemented using various op-amp configurations

---
## Results
- Frequency response of the 1st order EQ
![first order response](https://github.com/luke1241/ELEC2856_DSP_EQ/assets/95569413/03dcc18e-6b29-4b67-b3cd-d8e727e06d94)

- Frequency response of the 2nd order EQ
- ![second order response](https://github.com/luke1241/ELEC2856_DSP_EQ/assets/95569413/861bc882-a9ce-48c9-b5a0-4a1cbbe5cf26)
