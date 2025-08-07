# ESPHome / HomeA ssistant Samsung Heat Pump Integration

This component enables Samsung Heat Pumps to be integrated into ESPHome and Home Assistant. It supports NASA heat pumps that utilize the MIM-E03CN / MIM-E03DN & MIM-E03EN control boards (e.g., Samsung Gen 6 heat pump etc). Older non-NASA systems are not supported. 

The project requires a suitable RS485 Modbus enabled board such as the M5Stack Atomic RS485 Base (https://shop.m5stack.com/products/atomic-rs485-base) with the Atom Lite (https://shop.m5stack.com/products/atom-lite-esp32-development-kit). But any ESPHome supported board with a TTL-RS485 converter should theoretically work. Modbus A connects to F2 and Modubs B connects to F1 on the MIM control board. F1 & F2 are the connectors used for communication betweeen the internal unit/MIM board and the external heat pump unit. F3 & F4 are the connectors used by the wired LCD touch controllers. 

This project borrows from the excellent work done by the team supporting the ESPHome Samsung HVAC Integration (https://github.com/omerfaruk-aran/esphome_samsung_hvac_bus/). The project includes detailed [hardware installation instructions](https://github.com/omerfaruk-aran/esphome_samsung_hvac_bus/wiki/Hardware-Installation) on the installation process so I won't repeat them here.

Most of the useful controls for DHW (domestic hot water), and heating (single and 2-zone) are supported by this component - along with the ability to read and write FSVs (Field Setting Values). The project has been re-engineered to abstract away the NASA message codes from the C++ code so it should be much easier to add missing controls to the Python lists of ESPHome components (lists, numbers, selects, sensors etc). This should make it much easier to submit PRs for missing NASA messages.

All commands, and FSVs are implemented as standard ESPHome components (e.g., lists, numbers, selects, switches, sensors). Check out the [example.yaml](example.yaml).

## Basic Controller, NASA Client & Device Configuration

```yaml
samsung_nasa:
  debug_log_messages: false
  debug_log_undefined_messages: false
  nasa_client: {}
  devices:
   - address: 20.00.00
     id: nasa_device_1
   - address: 10.00.00
     id: nasa_device_2
```

20.00.00 would normally be the address of the indoor unit; 10.00.00 would be the address of the outdoor unit (heat pump). It's best to leave the nasa_client options empty (at their defaults) as the client has been tested with these default values. You will need to provide an id for each device so you can associate each device with an ESPHome component.

## Advanced Controller, NASA Client & Device Configuration

```yaml
samsung_nasa:
  debug_log_messages: false
  debug_log_undefined_messages: false
  nasa_client:
    silence_interval: 100
    retry_interval: 500
    min_retries: 1
    send_timeout: 1000
  devices:
   - address: 20.00.00
     id: nasa_device_1
   - address: 10.00.00
     id: nasa_device_2
```

The nasa_client configuration options are to do with ensuring NASA message delivery via a retry mechansim. Thanks and acknowledgment go to atanasenko for the [retry mechanism.](https://github.com/omerfaruk-aran/esphome_samsung_hvac_bus/commit/1030af3bcc4f3dc688be643e0c2ae65b6401fcc5)

 - **silence_interval**: The time to wait since the last wire activity before sending. 
 - **retry_interval**: The minimum time before a retry attempt.  
 - **min_retries**: The minimum number of retries, even beyond timeout. 
 - **send_timeout**: The maximum time to wait before discarding commands.  

## Number

Comamnds and FSVs are implemented as number components when they represent a range of values such as temperature, duration etc. For commands use the message option with the NASA hex code; for FSVs use the fsv field:

```yaml
number:
  - platform: samsung_nasa
    message: 0x4201
    name: "Zone 1 Target Temp"
    internal: true
    nasa_device_id: nasa_device_1
    id: zone_target_temp

  - platform: samsung_nasa
    fsv: 2011
    nasa_device_id: nasa_device_1
    name: "Water Law (Outdoor Temp) High"
    id: fsv_2011
```

These are the only fields you need to provide. All other fields such as unit of measure, decimal accuracy etc are automatically configured based on the message or fsv value. Number components are read/write so in the examples above the target temperature can be read and modified; likewise the FSV (Water Law (Outdoor Temp) High) can be modified and the new value sent to the heat pump. Caution needs to be exercised with FSVs. While care has been taken to limit the values to those that are appropriate for the given FSV, there can be minor differences between control boards and permitted min/max values.  

You can find a list of supported commands and FSVs in the [python configuration file for number components.](/components/samsung_nasa/nasa/numbers.py)

For example here are the python entries for the above number components:

```python
numbers = {
    0x4201: {
        NASA_LABEL: "VAR_IN_TEMP_TARGET_F",
        NASA_MODE: CONTROLLER_MODE_CONTROL,
        CONF_DATA: cmd_numeric_data(16, 30),
        CONF_DEFAULTS: temperature_defaults()
    },
    0x4254: {
        NASA_LABEL: "VAR_IN_FSV_2011",
        NASA_MODE: CONTROLLER_MODE_FSV,
        CONF_DATA: fsv_numeric_data(2011, -20, 5),
        CONF_DEFAULTS: temperature_defaults()
    }
}

```

## Select

```yaml
select:
  - platform: samsung_nasa
    message: 0x4066
    nasa_device_id: nasa_device_1
    name: Hot Water Mode
    id: hotwater_mode
```

The correct options for select components are automatically configured. You can find a list of supported select components and FSVs in the [python configuration file](/components/samsung_nasa/nasa/selects.py) that is used to auto generate the select component.

So for example message 0x4066 (hot water mode):

```python
0x4066: {
    NASA_LABEL: "ENUM_IN_WATER_HEATER_MODE",
    NASA_MODE: CONTROLLER_MODE_CONTROL,
    CONF_DATA: cmd_select_data(
        [
            "Economy", 
            "Standard", 
            "Power", 
            "Force"
        ]
    ),
    CONF_DEFAULTS: select_defaults()
}
```

## Switch

Binary type NASA commands and FSV values (such as ON/OFF, YES/NO, ENABLED/DISABLED) are represented as switches. Like Number and Select componentes, they are read/write.

```yaml
switch:
  - platform: samsung_nasa
    message: 0x4065
    nasa_device_id: nasa_device_1
    name: "DHW Power"
    internal: true
    id: dhw_power_switch
  - platform: samsung_nasa
    message: 0x4000
    nasa_device_id: nasa_device_1
    name: "Zone 1 Power"
    internal: true
    id: zone_power
```
 
 A list of available switches can be found in [switches.py](/components/samsung_nasa/nasa/switches.py) which is used to auto configure the switch components.

```python
switches = {
    0x4065: {
        NASA_LABEL: "ENUM_IN_WATER_HEATER_POWER",
        NASA_MODE: CONTROLLER_MODE_CONTROL,
        CONF_DATA: empty_data(),
        CONF_DEFAULTS: bool_defaults()
    },    
    0x4000: {
        NASA_LABEL: "ENUM_IN_OPERATION_POWER",
        NASA_MODE: CONTROLLER_MODE_CONTROL,
        CONF_DATA: empty_data(),
        CONF_DEFAULTS: bool_defaults()
    },
}
```

## Sensor

Sensor components are read-only. They report real-time data such as room temperature, energy consumption/production and valve/pump status.

```yaml
sensor:
  - platform: samsung_nasa
    message: 0x4237
    nasa_device_id: nasa_device_1
    name: "DHW Temperature"
    internal: true
    id: hot_water_current_temp
```

In addition to a large selection of available sensors, it is also possible to specify your own NASA code should it not be listed in [sensors.py](/components/samsung_nasa/nasa/sensors.py). You will need to provide the appropriate unit of measure, device class, decimal accuracy and lambdas to transform the raw NASA value to something meaningful. Consult the [ESPHome documentation](https://esphome.io/components/sensor/) for how to configure a sensor component. As part of the samsung_nasa platform you will need to specify message, nasa_device_id and platform fields.

Here is the python entry that configures the above sensor:

```python
0x4237: {
    NASA_LABEL: "VAR_IN_TEMP_WATER_TANK_F",
    NASA_MODE: CONTROLLER_MODE_STATUS,
    CONF_DEFAULTS: temp_sensor_defaults()
},
```

## Automation and FSVs

Unlike NASA sensors which regularly report data without being requested, FSV values need to be requested. The samsung_nasa.request_read action enables you to request FSV values using ESPHome's powerful automation triggers and conditions. For example you can periodically request readings using ESPHome's interval component:

```yaml
interval:
  - interval: 30min
    startup_delay: 30s
    then:
      - samsung_nasa.request_read:
          id: [fsv_2011, fsv_2012, fsv_2021, fsv_2022]
```

In the above automation FSV values are read 30 seconds after startup and then every 30 minutes (or for example once a day).

Alternatively you can implement a configuration button component that when pressed will perform a read request:

```yaml
button:
  - platform: template
    name: Request
    entity_category: config
    on_press:
      - samsung_nasa.request_read:
          id: [fsv_2011, fsv_2012, fsv_2021, fsv_2022]
```

## Climate Component

The samsung_nasa platform extends to the climate component which can report current DHW (domestic hot water) temperature and room temperature if you use the Samsung wired controller as a thermostat. For DHW and heating you can control the target temperature and even bind select components to auto generate presets.

```yaml
climate:
  - platform: samsung_nasa
    name: Hot Water
    id: climate_hot_water
    power_switch_id: dhw_power_switch
    current_temp_sensor_id: hot_water_current_temp
    target_temp_number_id: hot_water_target_temp
    custom_preset_select_id: hotwater_mode
    action_mode_sensor:
      id: three_way_valve
      mappings:
        0: CLIMATE_ACTION_IDLE
        1: CLIMATE_ACTION_HEATING
    visual:
      min_temperature: 30
      max_temperature: 60
      temperature_step:
        target_temperature: 0.5
        current_temperature: 0.1
```

In the above example the climate control can report/control heat/off mode by binding the power_switch_id field to the appropriate switch component:

```yaml
switch:
  - platform: samsung_nasa
    message: 0x4065
    nasa_device_id: nasa_device_1
    name: "DHW Power"
    internal: true
    id: dhw_power_switch
```

The climate component reports hot water temperature by binding the current_temp_sensor_id to the appropriate sensor component:

```yaml
sensor:
  - platform: samsung_nasa
    message: 0x4237
    nasa_device_id: nasa_device_1
    name: "DHW Temperature"
    internal: true
    id: hot_water_current_temp
```

Likewise, target temperature can be bound to a number components for reading/modifying the desired temperature:

```yaml
number:
  - platform: samsung_nasa
    message: 0x4235
    name: DHW Target Temperature
    internal: true
    nasa_device_id: nasa_device_1
    id: hot_water_target_temp
```
If you are feeling very adventurous you can even have the climate component report current action of the heat pump (idle, heating, off etc) by binding the action_mode_sensor field to the appropriate pump or valve. In the above example NASA code 0x4067 is the code for reporting the DHW valve status (0 = room; 1 = hot water tank). When this sensor reports a value of 1 the heat pump is actively heating the hot water tank.

```yaml
sensor:
    # 0x4067
    # 0 = room; 1 = tank
  - platform: samsung_nasa
    message: 0x4067
    nasa_device_id: nasa_device_1
    name: "3-Way Valve"
    internal: true
    id: three_way_valve 

climate:
  - platform: samsung_nasa
    name: Hot Water
    id: climate_hot_water
    power_switch_id: dhw_power_switch
    current_temp_sensor_id: hot_water_current_temp
    target_temp_number_id: hot_water_target_temp
    custom_preset_select_id: hotwater_mode
    action_mode_sensor:
      id: three_way_valve
      mappings:
        0: CLIMATE_ACTION_IDLE
        1: CLIMATE_ACTION_HEATING
```

Finally you populate the climate component's presets from a select component. In this example the NASA code 0x4066, which is implemented as a select component, is bound to the climate's custom_preset_select_id field.


```yaml
select:
  - platform: samsung_nasa
    message: 0x4066
    nasa_device_id: nasa_device_1
    name: Hot Water Mode
    id: hotwater_mode
```

An example heat pump dashboard in Home Assistant using this component and the example.yaml.

<img src="samsung_nasa.png" width="80%"/>
