#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#define PinEntrada  (P10_3)

/* Objeto ADC */
cyhal_adc_t adc_obj;

/* Objeto del Canal 0 del ADC */
cyhal_adc_channel_t adc_chan_0_obj;

int main(void)
{
	cy_rslt_t result;

	#if defined (CY_DEVICE_SECURE)
	    cyhal_wdt_t wdt_obj;
	    /* Limpiar el temporizador del watchdog para que no provoque un reinicio */
	    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
	    CY_ASSERT(CY_RSLT_SUCCESS == result);
	    cyhal_wdt_free(&wdt_obj);
	#endif

	    /* Inicializar los perifÃ©ricos del dispositivo y de la placa */
	    result = cybsp_init();

	    /* Enable global interrupts */
	    __enable_irq();

    /* Inicializar retarget-io para usar el puerto UART de depuraciÃ³n */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* Inicializar el ADC */
    cyhal_adc_init(&adc_obj, PinEntrada, NULL);

    /* ConfiguraciÃ³n del canal ADC */
    const cyhal_adc_channel_config_t channel_config = {
        .enable_averaging = false,   // Desactivar el promedio para el canal
        .min_acquisition_ns = 1000,  // Tiempo de adquisiciÃ³n mÃ­nimo establecido en 1 us
        .enabled = true             // Muestrear este canal cuando el ADC realice un escaneo
    };

    /* ConfiguraciÃ³n del canal ADC */
    cyhal_adc_channel_init_diff(&adc_chan_0_obj, &adc_obj, PinEntrada, CYHAL_ADC_VNEG, &channel_config);

    printf("Proporcione el voltaje de entrada en el pin de entrada. \r\n\n");

    for (int i = 0; i < 20; i++)
     {
         /* Leer el voltaje de entrada en microvoltios */
         int32_t adc_result = cyhal_adc_read_uv(&adc_chan_0_obj);

         /* Convertir el valor a milivoltios */
         int32_t voltage_in_millivolts = adc_result / 1000;

         /* Imprimir voltaje de entrada en milivoltios */
         printf("Lectura %d: Voltaje en P10_3: %d mV\r\n", i + 1, (int) voltage_in_millivolts);

         /* Retardo antes de la prÃ³xima lectura */
         cyhal_system_delay_ms(5000);
     }

    printf("Fin de lecturas. \r\n\n");
}
