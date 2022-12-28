// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <stdexcept>

void TBitField::Swap(TBitField& lhs, TBitField& rhs) {
    std::swap(lhs.bitLen, rhs.bitLen);
    std::swap(lhs.memLen, rhs.memLen);
    std::swap(lhs.pMem, rhs.pMem);
}

TBitField::TBitField(int len) : bitLen(len)
{
    if (len < 0) {
        throw std::out_of_range("Length can't be negative");
    }
    memLen = (len + bitsInElem - 1) >> shiftSize;
    pMem = new TELEM[memLen];
    memset(pMem, 0, memLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf)
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new TELEM[memLen];
    memcpy(pMem, bf.pMem, memLen * sizeof(TELEM));
}

TBitField::TBitField(TBitField&& bf)
{
    pMem = nullptr;
    Swap(*this, bf);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> shiftSize;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & (bitsInElem - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= bitLen)
        throw std::out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= bitLen)
        throw std::out_of_range("bit pos is out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= bitLen)
        throw std::out_of_range("bit pos is out of range");

    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    TBitField tmp(bf);
    Swap(*this, tmp);
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bitLen != bf.bitLen) return false;
    for (int i = 0; i < bf.memLen; i++)
        if (pMem[i] != bf.pMem[i])
            return false;
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField tmp(bitLen > bf.bitLen ? *this : bf);
    for (int i = 0; i < std::min(memLen, bf.memLen); i++)
        tmp.pMem[i] = pMem[i] | bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField tmp(bitLen > bf.bitLen ? *this : bf);
    for (int i = 0; i < std::min(memLen, bf.memLen); i++)
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
    for (int i = 0; i < tmp.bitLen; i++)
        if (GetBit(i)) {
            tmp.ClrBit(i);
        } else {
            tmp.SetBit(i);
        }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.bitLen; i++) {
        bool x;
        istr >> x;
        if (x) {
            bf.SetBit(i);
        } else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++) {
        ostr << (bf.GetBit(i) ? 1 : 0) << ' ';
    }
    return ostr;
}
