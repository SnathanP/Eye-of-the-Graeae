# include "property.h"

void front(double input[], double weights1[], double weights2[], /* RESULTS */ double hiddenSum[], double hiddenResult[], double outputSum[], double outputResult[]);
void back(double input[], double answer[], double weights1[], double weights2[], double hiddenSum[], double hiddenResult[], double outputSum[], double outputResult[]);
