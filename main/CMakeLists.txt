idf_component_register(
    SRCS 
        "main.cpp" 
        "sensors/bh1750.cpp"
    
    INCLUDE_DIRS 
        ""
        "sensors"

    PRIV_REQUIRES 
        esp_driver_i2c
        esp_driver_gpio
)
