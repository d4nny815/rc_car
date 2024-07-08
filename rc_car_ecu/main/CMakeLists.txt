# Register the main application component
idf_component_register(SRCS "led.c" "main.c" "potentiometer.c" "motor.c" "button.c"
                        INCLUDE_DIRS ".")
