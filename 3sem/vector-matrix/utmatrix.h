// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
// Верхнетреугольная матрица - реализация на основе шаблона вектора

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <string>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
  ValType *pVector;
  int Size;       // размер вектора
  int StartIndex; // индекс первого элемента вектора
public:
  TVector(int s = 10, int si = 0);          //                            (#О1)
  TVector(const TVector &v);                // конструктор копирования    (#Л1)
  virtual ~TVector();                               //                            (#О2)
  int GetSize() const; // размер вектора              (#О)
  int GetStartIndex() const; // индекс первого элемента     (#О)
  ValType& operator[](int index);             // доступ                     (#П2)
  ValType& operator[](int index) const;
  bool operator==(const TVector &v) const;  // сравнение                  (#П3)
  bool operator!=(const TVector &v) const;  // сравнение                  (#П3)
  TVector& operator=(const TVector &v);     // присваивание              (#О3)

  // скалярные операции
  TVector  operator+(const ValType &val) const;   // прибавить скаляр           (#Л2)
  TVector  operator-(const ValType &val) const;   // вычесть скаляр             (#С1)
  TVector  operator*(const ValType &val) const;   // умножить на скаляр         (#С2)

  // векторные операции
  TVector  operator+(const TVector &v) const;     // сложение                   (#С3)
  TVector  operator-(const TVector &v) const;     // вычитание                  (#С4)
  ValType  operator*(const TVector &v) const;     // скалярное произведение     (#С5)

  // ввод-вывод
  friend istream& operator>>(istream &in, TVector &v) //                  (#П4)
  {
    for (int i = 0; i < v.Size; i++) {
      cout << "Enter `" + to_string(i) + "` element:\n";  // TODO: finish this
    }
    return in;                                                      
  }
  friend ostream& operator<<(ostream &out, const TVector &v) //           (#С6)
  {
    for (int i = 0; i < v.StartIndex; i++) {
      cout << ValType() << ' ';
    }
    for (int i = 0; i < v.Size; i++) {
      cout << v.pVector[i] << ' ';
    }
    return out;                                                     // SKIP_OFF
  }
};

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
  if (s <= 0) {
    throw std::length_error("Size must be positive");
  }
  if (s > MAX_VECTOR_SIZE) {
    throw std::length_error("Size must be less that MAX_VECTOR_SIZE");
  }
  if (si < 0) {
    throw std::invalid_argument("StartIndex can't be negative");
  }

  this->Size = s;
  this->StartIndex = si;
  this->pVector = new ValType[this->Size]();
} /*-------------------------------------------------------------------------*/

template <class ValType> //конструктор копирования
TVector<ValType>::TVector(const TVector<ValType> &v)
{
  this->Size = v.Size;
  this->StartIndex = v.StartIndex;
  this->pVector = new ValType[v.Size]();

  for (int i = 0; i < this->Size; i++) {
    this->pVector[i] = v.pVector[i];
  }

} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>::~TVector()
{
  delete[] this->pVector;
} /*-------------------------------------------------------------------------*/

template <class ValType>
int TVector<ValType>::GetSize() const {
  return this->Size;
} /*-------------------------------------------------------------------------*/

template <class ValType>
int TVector<ValType>::GetStartIndex() const {
  return this->StartIndex;
} /*-------------------------------------------------------------------------*/

template <class ValType> // доступ
ValType& TVector<ValType>::operator[](int index)
{
  if (index < this->StartIndex || index >= this->Size + this->StartIndex) {
    throw std::invalid_argument("Index out of range");
  }

  return this->pVector[index - this->StartIndex];
} /*-------------------------------------------------------------------------*/

template <class ValType> // доступ
ValType& TVector<ValType>::operator[](int index) const
{
  if (index < this->StartIndex || index >= this->Size + this->StartIndex) {
    throw std::invalid_argument("Index out of range");
  }

  return this->pVector[index - this->StartIndex];
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator==(const TVector &v) const
{
  if (this == &v) {
    return true;
  }
  if (this->Size != v.Size || this->StartIndex != v.StartIndex) {
    return false;
  }

  return std::memcmp(this->pVector, v.pVector, this->Size) == 0;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator!=(const TVector &v) const
{
  return !(*this == v);
} /*-------------------------------------------------------------------------*/


template <class ValType> // присваивание
TVector<ValType>& TVector<ValType>::operator=(const TVector &v)
{
  if (v.Size != this->Size) {
    ValType* tmp = new ValType[v.Size];
    std::copy(v.pVector, v.pVector + v.Size, tmp);
    delete[] this->pVector;
    this->pVector = tmp;
  } else {
    std::copy(v.pVector, v.pVector + v.Size, this->pVector);
  }
  this->Size = v.Size;
  this->StartIndex = v.StartIndex;

  return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // прибавить скаляр
TVector<ValType> TVector<ValType>::operator+(const ValType &val) const
{
  TVector<ValType> out(*this);
  for (int i = 0; i < out.Size; i++) {
    out.pVector[i] += val;
  }
  return out;                                                      // SKIP_OFF
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычесть скаляр
TVector<ValType> TVector<ValType>::operator-(const ValType &val) const
{
  TVector<ValType> out(*this);
  for (int i = 0; i < out.Size; i++) {
    out.pVector[i] -= val;
  }
  return out;                                                      // SKIP_OFF
} /*-------------------------------------------------------------------------*/

template <class ValType> // умножить на скаляр
TVector<ValType> TVector<ValType>::operator*(const ValType &val) const
{
  TVector<ValType> out(*this);
  for (int i = 0; i < out.Size; i++) {
    out.pVector[i] *= val;
  }
  return out;                                                      // SKIP_OFF

} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType> &v) const
{
  if (this->Size != v.Size) {
    throw std::length_error("Lengths don't match");
  }
  TVector<ValType> out(*this);
  for (int i = 0; i < this->Size; i++) {
    out.pVector[i] += v.pVector[i];
  }
  return out;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType> &v) const
{
  if (this->Size != v.Size) {
    throw std::length_error("Lengths don't match");
  }
  TVector<ValType> out(*this);
  for (int i = 0; i < this->Size; i++) {
    out.pVector[i] -= v.pVector[i];
  }
  return out;
} /*-------------------------------------------------------------------------*/

template <class ValType> // скалярное произведение
ValType TVector<ValType>::operator*(const TVector<ValType> &v) const
{
  if (this->Size != v.Size) {
    throw std::length_error("Lengths don't match");
  }
  ValType sum = ValType();
  for (int i = 0; i < this->Size; i++) {
    sum += this->pVector[i] * v.pVector[i];
  }
  return sum;
} /*-------------------------------------------------------------------------*/


// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType>>
{
public:
  TMatrix(int s = 10);                           //                       (#О1)
  TMatrix(const TMatrix &mt);                    // копирование           (#Л1)
  TMatrix(const TVector<TVector<ValType> > &mt); // преобразование типа   (#Л2)
  bool operator==(const TMatrix &mt) const;      // сравнение             (#П1)
  bool operator!=(const TMatrix &mt) const;      // сравнение             (#П1)
  TMatrix& operator= (const TMatrix &mt);        // присваивание          (#О2)
  TMatrix  operator+ (const TMatrix &mt) const;        // сложение              (#П2)
  TMatrix  operator- (const TMatrix &mt) const;        // вычитание             (#С1)
  TMatrix  operator* (const TMatrix &mt) const;        // умножение

  // ввод / вывод
  friend istream& operator>>(istream &in, TMatrix &mt)                //  (#П3)
  {
    // for (int y = 0; y < mt.GetSize(); y++) {
    //   for (int x = 0; x < mt[y].GetSize(); x++) {
    //     std::cout << "Enter matrix[" + std::to_string(y) + "][" + std::to_string(x) + "]: \n";
    //     cin >> mt[y][x];  // TODO: maybe validate input?
    //   }
    // }
    return in;
  }
  friend ostream & operator<<(ostream &out, const TMatrix &mt) //        (#С3)
  {                                
    for (int i = 0; i < mt.Size; i++)
      cout << mt.pVector[i] << '\n';
    return out;
  }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s) {
  if (s*s > MAX_MATRIX_SIZE) {
    throw std::length_error("Size must be less that MAX_MATRIX_SIZE");
  }
  for (int i = 0; i < s; i++) {
    this->pVector[i] = TVector<ValType>(s - i, i);
  }
}
template <class ValType> // конструктор копирования
TMatrix<ValType>::TMatrix(const TMatrix<ValType> &mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType> // конструктор преобразования типа
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>> &mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType> // сравнение
bool TMatrix<ValType>::operator==(const TMatrix<ValType> &mt) const
{
  if (this == &mt) return true;
  if (this->GetSize() != mt.GetSize()) return false;

  for (int i = 0; i < this->Size; i++) {
    if (this->pVector[i] != mt.pVector[i]) {
      return false;
    }
  }
  return true;
} 

template <class ValType> // сравнение
bool TMatrix<ValType>::operator!=(const TMatrix<ValType> &mt) const
{
  return !(*this == mt);
} 
template <class ValType> // присваивание
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType> &mt)
{
  if (this != &mt) {
    if (this->Size != mt.GetSize()) {
      delete[] this->pVector;
      this->pVector = new TVector<ValType>[mt.GetSize()];
    }

    this->Size = mt.GetSize();
    for (int i = 0; i < this->Size; i++) {
      this->pVector[i] = mt[i];
    }
  }
  return *this;
}
template <class ValType> // сложение
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType> &mt) const
{
  if (this->Size != mt.Size) {
    throw std::length_error("Lengths of matrixes don't match");
  }
  TMatrix<ValType> out(*this);
  for (int i = 0; i < this->Size; i++) {
    out.pVector[i] = this->pVector[i] + mt.pVector[i];
  }

  return out;
}

template <class ValType> // вычитание
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType> &mt) const
{
  if (this->Size != mt.Size) {
    throw std::length_error("Lengths of matrixes don't match");
  }
  TMatrix<ValType> out(*this);
  for (int i = 0; i < this->Size; i++) {
    out.pVector[i] = this->pVector[i] - mt.pVector[i];
  }
  return out;
} 
template <class ValType> // умножение
TMatrix<ValType> TMatrix<ValType>::operator*(const TMatrix<ValType> &mt) const
{
  if (this->Size != mt.Size) {
    throw std::length_error("Lengths of matrixes don't match");
  }
  TMatrix<ValType> out(this->Size);
  for (int i = 0; i < this->Size; i++) {
    for (int j = i; j < this->Size; j++) {
      ValType sum = ValType();
      for (int k = i; k < j + 1; k++) {
        sum += (*this)[i][k] * mt[k][j];
      }
      out[i][j] = sum;
    }
  }
  return out;
}

#endif
