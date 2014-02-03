//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2009 - Julien Fryer - julienfryer@gmail.com				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////


#include "../../Extensions/Zones/SPK_Plane.h"

namespace SPK
{
	Plane::Plane(const Vector3D& position,const Vector3D& normal) :
		Zone(position)
	{
		setNormal(normal);
	}

	bool Plane::intersects(const Vector3D& v0,const Vector3D& v1,Vector3D* intersection,Vector3D* normal) const
	{
		float dist0 = dotProduct(tNormal,v0 - getTransformedPosition());
		float dist1 = dotProduct(tNormal,v1 - getTransformedPosition());

		if ((dist0 <= 0.0f) == (dist1 <= 0.0f)) // both points are on the same side
			return false;

		if (intersection != NULL)
		{
			if (dist0 <= 0.0f)
				dist0 = -dist0;
			else
				dist1 = -dist1;

			if (normal != NULL)
				*normal = tNormal;

			float ti = dist0 / (dist0 + dist1);

			Vector3D vDir = v1 - v0;
			float norm = vDir.getNorm();

			norm *= ti;
			ti = norm < APPROXIMATION_VALUE ? 0.0f : ti * (norm - APPROXIMATION_VALUE) / norm;

			vDir *= ti;
			*intersection = v0 + vDir;
		}

		return true;
	}

	void Plane::moveAtBorder(Vector3D& v,bool inside) const
	{
		float dist = dotProduct(tNormal,v - getTransformedPosition());

		if ((dist <= 0.0f) == inside)
			inside ? dist += APPROXIMATION_VALUE : dist -= APPROXIMATION_VALUE;
		else
			inside ? dist -= APPROXIMATION_VALUE : dist += APPROXIMATION_VALUE;

		v += tNormal * -dist;
	}

	void Plane::innerUpdateTransform()
	{
		Zone::innerUpdateTransform();
		transformDir(tNormal,normal);
		tNormal.normalize();
	}
}
