class Vector:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)

    def _sqrt(self, x):
    	if x < 0:
    		raise ValueError('x < 0')

    	return x**0.5
        
    def __add__(self, vec):
        return Vector(self.x+vec.x, self.y+vec.y, self.z+vec.z)
        
    def __iadd__(self, vec):
        self.__add__(vec)
    
    def __sub__(self, vec):
        return Vector(self.x-vec.x, self.y-vec.y, self.z-vec.z)
    
    def __isub__(self, vec):
        self.__sub__(vec)
    
    def __neg__(self):
        return Vector(-self.x, -self.y, -self.z)
    
    def __eq__(self, vec):
        return self.x == vec.x and self.y == vec.y and self.z == vec.z
        
    def __mul__(self, fl):
        return Vector(self.x * fl, self.y * fl, self.z * fl)
    
    def __imul__(self, fl):
        return self.__mul__(fl)
        
    def lenght(self):
        return self._sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

    def tuple(self):
   		return (self.x, self.y, self.z)

    def normalize(self):
        lenght = self.lenght()

        if lenght == 0:
            return self

        lenght = 1/lenght
        return Vector( self.x * lenght, self.y * lenght, self.z * lenght )
        
    def __repr__(self):
        return '({}, {}, {})'.format(self.x, self.y, self.z)

    def __str__(self):
    	return self.__repr__()


def UTIL_ClampVectorToBox(vecInput, clampSize):
    sourceVector = Vector(vecInput.x, vecInput.y, vecInput.z);

    if( sourceVector.x > clampSize.x ):
        sourceVector.x -= clampSize.x;
    elif( sourceVector.x < -clampSize.x ):
        sourceVector.x += clampSize.x;
    else:
        sourceVector.x = 0;

    if( sourceVector.y > clampSize.y ):
        sourceVector.y -= clampSize.y;
    elif( sourceVector.y < -clampSize.y ):
        sourceVector.y += clampSize.y;
    else:
        sourceVector.y = 0;

    if( sourceVector.z > clampSize.z ):
        sourceVector.z -= clampSize.z;
    elif( sourceVector.z < -clampSize.z ):
        sourceVector.z += clampSize.z;
    else:
        sourceVector.z = 0;

    return sourceVector.normalize();

SET_GLOBAL('Vector', Vector)
SET_GLOBAL('UTIL_ClampVectorToBox', UTIL_ClampVectorToBox)
SET_GLOBAL('DotProduct', lambda a, b: a.x * b.x + a.y * b.y + a.z * b.z)