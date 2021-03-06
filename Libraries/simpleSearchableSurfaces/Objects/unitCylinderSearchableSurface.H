/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2008 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::unitCylinderSearchableSurface

Description
    Searching on sphere

SourceFiles
    unitCylinderSearchableSurface.C

Contributors/Copyright:
    2009, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id$
\*---------------------------------------------------------------------------*/

#ifndef unitCylinderSearchableSurface_H
#define unitCylinderSearchableSurface_H

#include "searchableSurface.H"
#include "treeBoundBox.H"
#include "plane.H"

#include "swak.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class unitCylinderSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class unitCylinderSearchableSurface
:
    public searchableSurface
{
private:

    // Private Member Data

    plane top_;
    plane bottom_;

    //- Names of regions
    mutable wordList regions_;


    // Private Member Functions

        //- Find nearest point on sphere.
        pointIndexHit findNearest
        (
            const point& sample,
            const scalar nearestDistSqr
        ) const;

        //- Find intersections with cylindee
        void findLineAll
        (
            const point& start,
            const point& end,
            List<pointIndexHit>& hit
        ) const;


        //- Disallow default bitwise copy construct
        unitCylinderSearchableSurface(const unitCylinderSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const unitCylinderSearchableSurface&);


public:

    //- Runtime type information
    TypeName("unitCylinder");


    // Constructors

        //- Construct from components
        unitCylinderSearchableSurface(const IOobject& io, const point&, const scalar radius);

        //- Construct from dictionary (used by searchableSurface)
        unitCylinderSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~unitCylinderSearchableSurface();


    // Member Functions

        virtual label size() const { return 3; }

#ifdef FOAM_SEARCHABLE_SURF_USES_TMP
        virtual tmp<pointField> coordinates() const;
#else
        virtual pointField coordinates() const;
#endif

        virtual tmp<pointField> points() const;

        virtual bool overlaps(const boundBox &) const;

        virtual const wordList& regions() const;

        //- Whether supports volume type below
        virtual bool hasVolumeType() const
        {
            return true;
        }


        // Multiple point queries.

            virtual void findNearest
            (
                const pointField& sample,
                const scalarField& nearestDistSqr,
                List<pointIndexHit>&
            ) const;

            virtual void findLine
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const;

            virtual void findLineAny
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const;

            //- Get all intersections in order from start to end.
            virtual void findLineAll
            (
                const pointField& start,
                const pointField& end,
                List<List<pointIndexHit> >&
            ) const;

            //- From a set of points and indices get the region
            virtual void getRegion
            (
                const List<pointIndexHit>&,
                labelList& region
            ) const;

            //- From a set of points and indices get the normal
            virtual void getNormal
            (
                const List<pointIndexHit>&,
                vectorField& normal
            ) const;

            //- Determine type (inside/outside/mixed) for point. unknown if
            //  cannot be determined (e.g. non-manifold surface)
            virtual void getVolumeType
            (
                const pointField&,
                List<volumeType>&
            ) const;

        // regIOobject implementation

            bool writeData(Ostream&) const
            {
                notImplemented("unitCylinderSearchableSurface::writeData(Ostream&) const");
                return false;
            }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
