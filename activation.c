double sigmoid(double x) {
    return 1.0/(1.0 + exp(-x));
}

double sigmoid_prime(double x) {
    return sigmoid(x)*(1.0-sigmoid(x));
}

double tanh(double x) {
    return tanh(x);
}

double tanh_prime(double x) {
    return 1.0 - pow(x,2);
}
