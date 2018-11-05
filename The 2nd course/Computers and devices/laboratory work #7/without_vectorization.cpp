#include <iostream>
using namespace std;

class Matrix {
protected:
    float** matrix_;
    int order_;
public:
    explicit Matrix(int order);

    ~Matrix();

    friend istream& operator>>(istream& s, Matrix& matrix);

    friend ostream& operator<<(ostream& s, Matrix& matrix);

    Matrix operator+(Matrix& b);

    Matrix operator*(Matrix& b);

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

Matrix::~Matrix(){
    for(int i = 0; i < order_; i++)
        delete[] matrix_[i];
    delete[] matrix_;
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

Matrix Matrix::operator+(Matrix& b){
    if(order_ != b.order_)
        throw logic_error("Error: orders are different");
    Matrix tmp(order_);
    for(int i = 0; i < order_; i++)
        for(int j = 0; j < order_; j++)
            tmp.matrix_[i][j] = matrix_[i][j] + b.matrix_[i][j];
    return tmp;
}

Matrix Matrix::operator*(Matrix& b){
    if(order_ != b.order_)
        throw logic_error("Error: orders are different");
    Matrix tmp(order_);
    for(int i = 0; i < order_; i++)
        for(int j = 0; j < order_; j++)
            for(int k = 0; k < order_; k++)
                tmp.matrix_[i][j] += matrix_[i][k] * b.matrix_[k][j];
    return tmp;
}

int main(){
    Matrix matrix_1(2);
    Matrix matrix_2(2);

    cin >> matrix_1;
    cin >> matrix_2;

    Matrix matrix_3 = matrix_1 * matrix_2;
    cout << endl << matrix_3;

    return 0;
}
