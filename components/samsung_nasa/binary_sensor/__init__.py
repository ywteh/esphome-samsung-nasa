import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_DEFAULTS,CONF_FILTERS
from ..nasa.nasa import NASA_BinarySensor
from ..nasa.const import (
    NASA_LABEL,
    NASA_MESSAGE,
    NASA_MODE,
)
from .. import (
    nasa_item_base_schema,
    NASA_CONTROLLER_ID,
    NASA_DEVICE_ID
)
from ..nasa.binary_sensors import binary_sensors


AUTO_LOAD = ["samsung_nasa"]
DEPENDENCIES = ["samsung_nasa"]

def validate(config):
    if NASA_MESSAGE in config:
        if (nasa_binary_sensor := binary_sensors.get(config[NASA_MESSAGE])) is not None:
            config[NASA_LABEL] = nasa_binary_sensor[NASA_LABEL]
            config[NASA_MODE] = nasa_binary_sensor[NASA_MODE]
            if (conf_defaults := nasa_binary_sensor.get(CONF_DEFAULTS)) is not None:
                conf_defaults = conf_defaults()
                if (filters := conf_defaults.get(CONF_FILTERS)) is not None:
                    filters.extend(config.get(CONF_FILTERS,[]))
                for key, value in conf_defaults.items():
                    config[key] = value
                config[CONF_FILTERS] = filters
        label = "Auto" if nasa_binary_sensor else "User"
        cv._LOGGER.log(
                cv.logging.INFO, 
                "{} configured NASA message {} as binary sensor component"
                .format(label, config[NASA_MESSAGE])
        )
    return config

nasa_schema = cv.All(
    cv.Schema(
        {cv.Required(NASA_MESSAGE): cv.hex_int},
        extra=cv.ALLOW_EXTRA
    ),
    validate
)

CONFIG_SCHEMA = cv.All(
    nasa_schema,
    binary_sensor.binary_sensor_schema(NASA_BinarySensor)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(NASA_BinarySensor),
            cv.Required(NASA_MESSAGE): cv.hex_int,
            cv.Optional(CONF_FILTERS): binary_sensor.validate_filters
        }
    )
    .extend(nasa_item_base_schema)
)

async def to_code(config):
    device = await cg.get_variable(config[NASA_DEVICE_ID])
    controller = await cg.get_variable(config[NASA_CONTROLLER_ID])
    var_binary_sensor = await binary_sensor.new_binary_sensor(
        config,
        config[NASA_LABEL],
        config[NASA_MESSAGE],
        config[NASA_MODE],
        device
    )
    cg.add(var_binary_sensor.set_parent(controller))
    cg.add(controller.register_component(var_binary_sensor))