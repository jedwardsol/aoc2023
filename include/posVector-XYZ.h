#pragma once

template <typename T=int>
struct Vector
{
    T dx{};
    T dy{};
    T dz{};

    Vector  &operator+=(Vector const &rhs)
    {
        dx+=rhs.dx;
        dy+=rhs.dy;
        dz+=rhs.dz;
        return *this;
    }

    Vector  &operator-=(Vector const &rhs)
    {
        dx-=rhs.dx;
        dy-=rhs.dy;
        dz-=rhs.dz;
        return *this;
    }

    friend bool operator== (Vector const &lhs,Vector const &rhs)=default;
    friend bool operator!= (Vector const &lhs,Vector const &rhs)=default;
    friend auto operator<=>(Vector const &lhs,Vector const &rhs)=default;
};


template <typename T=int>
struct Pos
{
    T x{};                             
    T y{};                             
    T z{};                             

    Pos  &operator+=(Vector<T> const &rhs)
    {
        x+=rhs.dx;
        y+=rhs.dy;
        z+=rhs.dz;
        return *this;
    }

    Pos  &operator-=(Vector<T> const &rhs)
    {
        x-=rhs.dx;
        y-=rhs.dy;
        z-=rhs.dz;
        return *this;
    }

    friend bool operator== (Pos const &lhs,Pos const &rhs)=default;
    friend bool operator!= (Pos const &lhs,Pos const &rhs)=default;
    friend auto operator<=>(Pos const &lhs,Pos const &rhs)=default;
};

template <typename T>
Vector<T> operator+(Vector<T>  lhs,Vector<T> const &rhs)
{
    return lhs+=rhs;
}

template <typename T>
Vector<T> operator-(Vector<T>  lhs,Vector<T> const &rhs)
{
    return lhs-=rhs;
}


template <typename T>
Pos<T> operator+(Pos<T>  lhs,Vector<T> const &rhs)
{
    return lhs+=rhs;
}

template <typename T>
Pos<T> operator-(Pos<T>  lhs,Vector<T> const &rhs)
{
    return lhs-=rhs;
}

template <typename T>
Pos<T> operator+(Vector<T> const &lhs,Pos<T> rhs)
{
    return rhs+=lhs;
}

template <typename T>
Vector<T> operator-(Pos<T> const &lhs,Pos<T> const &rhs)
{
    return {lhs.x-rhs.x,lhs.y-rhs.y, lhs.z-rhs.z};
}
