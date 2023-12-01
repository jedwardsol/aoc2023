#pragma once

struct Vector
{
    int dx{};
    int dy{};

    constexpr Vector  &operator+=(Vector const &rhs)
    {
        dx+=rhs.dx;
        dy+=rhs.dy;
        return *this;
    }

    constexpr Vector  &operator-=(Vector const &rhs)
    {
        dx-=rhs.dx;
        dy-=rhs.dy;
        return *this;
    }

    friend bool operator== (Vector const &lhs,Vector const &rhs)=default;
    friend bool operator!= (Vector const &lhs,Vector const &rhs)=default;
    friend auto operator<=>(Vector const &lhs,Vector const &rhs)=default;
};


struct Pos
{
    int x{};
    int y{};

    constexpr Pos  &operator+=(Vector const &rhs)
    {
        x+=rhs.dx;
        y+=rhs.dy;
        return *this;
    }

    constexpr Pos  &operator-=(Vector const &rhs)
    {
        x-=rhs.dx;
        y-=rhs.dy;
        return *this;
    }

    friend bool operator== (Pos const &lhs,Pos const &rhs)=default;
    friend bool operator!= (Pos const &lhs,Pos const &rhs)=default;
    friend auto operator<=>(Pos const &lhs,Pos const &rhs)=default;
};


constexpr Vector operator+(Vector  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

constexpr Vector operator-(Vector  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


constexpr Pos operator+(Pos  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

constexpr Pos operator-(Pos  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


constexpr Pos operator+(Vector const &lhs,Pos rhs)
{
    return rhs+=lhs;
}

constexpr Vector operator-(Pos const &lhs,Pos const &rhs)
{
    return {lhs.x-rhs.x,lhs.y-rhs.y};
}

