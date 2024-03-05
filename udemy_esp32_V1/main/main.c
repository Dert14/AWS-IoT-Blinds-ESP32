#include <stepper.h>
#include "aws_connection.h"

void app_main(void)
{
	aws_connect();
	init_base();
	move_stepper();
}
