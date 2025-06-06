/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "int64.H"
#include "IOstreams.H"

#include <inttypes.h>
#include <sstream>
#include <cerrno>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::word Foam::name(const int64_t val)
{
    std::ostringstream buf;
    buf << val;
    return buf.str();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>(Istream& is, int64_t& i)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isInteger64())
    {
        i = t.integer64Token();
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected int64_t, found " << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, int64_t&)");

    return is;
}


int64_t Foam::readInt64(Istream& is)
{
    int64_t val;
    is >> val;

    return val;
}


bool Foam::read(const char* buf, int64_t& s)
{
    char *endptr = nullptr;
    errno = 0;
    intmax_t l = strtoimax(buf, &endptr, 10);
    s = int64_t(l);
    return (*endptr == 0) && (errno == 0);
}


Foam::Ostream& Foam::operator<<(Ostream& os, const int64_t i)
{
    os.write(i);
    os.check("Ostream& operator<<(Ostream&, const int64_t)");
    return os;
}

#if defined(darwin64)
Foam::Istream& Foam::operator>>(Istream& is, long& val)
{
    return operator>>(is, reinterpret_cast<int64_t&>(val));
}

Foam::Ostream& Foam::operator<<(Ostream& os, const long val)
{
    return (os << int64_t(val));
}
#endif


// ************************************************************************* //
