#pragma once


struct Vector
{
    int dRow{};
    int dCol{};

    Vector  &operator+=(Vector const &rhs)
    {
        dRow+=rhs.dRow;
        dCol+=rhs.dCol;
        return *this;
    }

    Vector  &operator-=(Vector const &rhs)
    {
        dRow-=rhs.dRow;
        dCol-=rhs.dCol;
        return *this;
    }

    friend bool operator== (Vector const &lhs,Vector const &rhs)=default;
    friend bool operator!= (Vector const &lhs,Vector const &rhs)=default;
    friend auto operator<=>(Vector const &lhs,Vector const &rhs)=default;
};


struct Pos
{
    int row{};                           
    int col{};                           

    Pos  &operator+=(Vector const &rhs)
    {
        row+=rhs.dRow;
        col+=rhs.dCol;
        return *this;
    }

    Pos  &operator-=(Vector const &rhs)
    {
        row-=rhs.dRow;
        col-=rhs.dCol;
        return *this;
    }

    friend bool operator== (Pos const &lhs,Pos const &rhs)=default;
    friend bool operator!= (Pos const &lhs,Pos const &rhs)=default;
    friend auto operator<=>(Pos const &lhs,Pos const &rhs)=default;
};


inline Vector operator+(Vector  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

inline Vector operator-(Vector  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


inline Pos operator+(Pos  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

inline Pos operator-(Pos  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


inline Pos operator+(Vector const &lhs,Pos rhs)
{
    return rhs+=lhs;
}




inline Vector operator-(Pos const &lhs,Pos const &rhs)
{
    return {lhs.row-rhs.row,lhs.col-rhs.col};
}

#include <vector>
#include <span>

template<typename T>
struct Grid
{
    Grid(int width, int height, T  init=T{}) : width{width},height{height},data(width*height, init)
    {}

    auto operator[](int row)
    {
        return std::span<T>{data.begin()+(row*width),
                            data.begin()+((row+1)*width)};
    }

    auto operator[](int row) const
    {
        return std::span<T const>{data.begin()+(row*width),
                                  data.begin()+((row+1)*width)};
    }

    auto &operator[](Pos pos)
    {
        return data[ pos.row*width + pos.col];
    }

    auto &at(Pos pos)
    {
        return data[ pos.row*width + pos.col];
    }


    auto &operator[](Pos pos) const
    {
        return data[ pos.row*width + pos.col];
    }


    bool inGrid(Pos pos) const
    {
        return     pos.row >= 0
                && pos.col >= 0
                && pos.row < height
                && pos.col < width;
    }


    int                 width;
    int                 height;
    std::vector<T>      data;
};
