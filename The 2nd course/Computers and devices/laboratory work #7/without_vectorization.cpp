#include <iostream>
using namespace std;

class Matrix {
private:
    float** matrix_;
    int order_;
public:
    Matrix(int order);

    friend istream& operator>>(istream& s, Matrix& matrix);

    friend ostream& operator<<(ostream& s, Matrix& matrix);
};

Matrix::Matrix(int order){
    order_ = order;
    matrix_ = new float*[order_];
    for(int i = 0; i < order_; i++){
        matrix_[i] = new float[order_];
        for(int j = 0; j < order_; j++)
            matrix_[i][j] = 0;
    }
}

istream& operator>>(istream& s, Matrix& matrix){
    for(int i = 0; i < matrix.order_; i++)
        for(int j = 0; j < matrix.order_; j++)
            s >> matrix.matrix_[i][j];
    return s;
}

ostream& operator<<(ostream& s, Matrix& matrix){
    for(int i = 0; i < matrix.order_; i++){
        for(int j = 0; j < matrix.order_; j++)
            s << matrix.matrix_[i][j] << " ";
        s << endl;
    }
    return s;
}

int main(){
    Matrix matrix(3);

    cin >> matrix;

    cout << endl << matrix;

    return 0;
}
