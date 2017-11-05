#include "SmartGradients.h"
#include "PlugInHostCommunication.h"

#pragma region GRADIENTSTATE

void InitializeAnchorPixel(GradientState* state,
    int32 num, int32 x, int32 y, uint8 ch0, uint8 ch1, uint8 ch2, uint8 ch3, uint8 ch4)
{
    state->AnchPixs[num].X = x;
    state->AnchPixs[num].Y = y;
    state->AnchPixs[num].CH[0] = ch0;
    state->AnchPixs[num].CH[1] = ch1;
    state->AnchPixs[num].CH[2] = ch2;
    state->AnchPixs[num].CH[3] = ch3;
    state->AnchPixs[num].CH[4] = ch4;
}

int32 ValidateGradientState(GradientState* state)
{
    if (state->Type == GradientType::COMPLEX)
    {
        if (state->ActiveAnchPixs < 3)
        {
            return AnchorPixNumUnderMinimumForComplex;
        }
        if (state->Px < 1 || state->Py < 1)
        {
            return PxPyUnderMinimumForComplex;
        }
        if (state->Parameter == ParameterNotUsed)
        {
            if (state->ActiveAnchPixs > 7)
            {
                return AnchorPixNumAboveMaxForComplexNoParam;
            }
            if (state->Px + state->Py + 1 != state->ActiveAnchPixs)
            {
                return PxPyMismatchForComplexNoParam;
            }
        }
        else
        {
            if (state->ActiveAnchPixs > 6)
            {
                return AnchorPixNumAboveMaxForComplexWithParam;
            }
            if (state->Px + state->Py != state->ActiveAnchPixs)
            {
                return PxPyMismatchForComplexWithParam;
            }
            if (state->Parameter < 0 || state->Parameter>400)
            {
                return ParameterOutOfRange;
            }
        }
    }
    else
    {
        if (state->ActiveAnchPixs < 2)
        {
            return AnchorPixNumUnderMinimumForElse;
        }
        if (state->ActiveAnchPixs > AnchorPixelMax)
        {
            return AnchorPixNumAboveMaximumForElse;
        }
        if (state->Type == GradientType::RADIAL)
        {
            if (state->CentralPix > state->ActiveAnchPixs - 1)
            {
                return CentralPixelAboveMaxForRadial;
            }
        }
    }

    if (state->DiffusionIndex < 0)
    {
        return DiffusionIndexNegative;
    }

    if (state->Angle == 0)
    {
        state->Angle = 0.0001;
    }

    return GradientStateNoErr;
}

void ResampleGradientState(GradientState* state, VRect renderingRect)
{
    for (int32 i = 0; i <= state->ActiveAnchPixs - 1; i++)
    {
        state->AnchPixs[i].X = state->AnchPixs[i].X - renderingRect.left;
        state->AnchPixs[i].Y = state->AnchPixs[i].Y - renderingRect.top;
    }
    state->CenterX = state->CenterX - renderingRect.left;
    state->CenterY = state->CenterY - renderingRect.top;
}

void ResampleGradientState(GradientState* state, VRect renderingRect, double zoom)
{
    for (int32 i = 0; i < state->ActiveAnchPixs; i++)
    {
        state->AnchPixs[i].X = (int32)(zoom * (double)state->AnchPixs[i].X);
        state->AnchPixs[i].Y = (int32)(zoom * (double)state->AnchPixs[i].Y);
    }
    state->CenterX = (int32)(zoom * (double)state->CenterX);
    state->CenterY = (int32)(zoom * (double)state->CenterY);

    for (int32 i = 0; i <= state->ActiveAnchPixs - 1; i++)
    {
        state->AnchPixs[i].X = state->AnchPixs[i].X - renderingRect.left;
        state->AnchPixs[i].Y = state->AnchPixs[i].Y - renderingRect.top;
    }
    state->CenterX = state->CenterX - renderingRect.left;
    state->CenterY = state->CenterY - renderingRect.top;
}

#pragma endregion


#pragma region HELPERS

double inline
#ifdef _M_IX86
__declspec (naked) __fastcall
#endif
asmsqrt(double n)
{
#ifdef _M_IX86
    _asm fld qword ptr[esp + 4]
        _asm fsqrt
    _asm ret 8
#else
    return sqrt(n);
#endif
}

uint8 inline ToByte(double input)
{
    if (input < 0)   return 0;
    if (input > 255) return 255;
                     return (uint8)input;
}

int32 inline Signum(double x)
{
    return (x > 0) - (x < 0);
}

bool CheckIfArrayContains(int32 value, int32* int32Array, int32 arrayLength)
{
    bool contains = false;
    for (int k = 0; k <= arrayLength - 1; k++)
    {
        if (int32Array[k] == value)
        {
            contains = true;
            break;
        }
    }
    return contains;
}

int32 GetArrayMin(int32* int32Array, int32 num)
{
    int32 min = INT_MAX;
    for (int32 i = 0; i <= num - 1; i++)
    {
        if (min > int32Array[i])
        {
            min = int32Array[i];
        }
    }
    return min;
}

int32 GetArrayMinLocationButIgnoreValues(int32* int32Array, int32* ignoredLocations, int32 arrayLenght, int32 ignoredLocationsLenght)
{
    int32 min = INT_MAX;
    int32 min_i = -1;
    for (int32 i = 0; i <= arrayLenght - 1; i++)
    {
        if (!CheckIfArrayContains(i, ignoredLocations, ignoredLocationsLenght))
        {
            if (min > int32Array[i])
            {
                min = int32Array[i];
                min_i = i;
            }
        }

    }
    return min_i;
}

void GetLinearRank(int32* int32Array, int32* rankArray, int32 arrayLenght)
{
    for (int32 i = 0; i <= arrayLenght - 1; i++)
        rankArray[i] = GetArrayMinLocationButIgnoreValues(int32Array, rankArray, arrayLenght, i);
}

#pragma endregion


#pragma region COMPLEX

double inline Transpose(uint8 C, double k)
{
    return(pow((double(C) / 255.0), k) * 255.0);
}

double inline UnTranspose(double xC, double k)
{
    if (xC <= 0) return 0;
    return pow(xC / 255.0, 1 / k) * 255.0;
}

int32 BinomialQuotient(int32 above, int32 below)
{
    int32 resultAbove = 1;
    int32 resultBelow = 1;
    int32 a           = above;
    int32 u           = 1;

    for (int32 i = 1; i <= below; i++)
    {
        resultAbove *= a; a -= 1;
        resultBelow *= u; u += 1;
    }

    return resultAbove / resultBelow;
}

double Polynomial(VectorXd* quotients, int32 x, int32 degree)
{
    double res = (*quotients)(0);
    double productX = 1;
    for (int32 i = 1; i <= degree; i++)
    {
        productX *= x;
        res  += (*quotients)(i) * productX;
    }
    return res;
}

MatrixXd GetMainMatrix(GradientState* state)
{
    int32 activeAnchPixs = state->ActiveAnchPixs;
    AnchorPixel* anchPixs = &(state->AnchPixs[0]);
    uint8 Px = state->Px;
    uint8 Py = state->Py;

    MatrixXd matrix(activeAnchPixs, activeAnchPixs);

    for (int32 i = 0; i <= Px - 1; i++)
        for (int32 j = 0; j <= activeAnchPixs - 1; j++)
            matrix(j, i) = pow(anchPixs[j].X, i + 1);

    for (int32 i = 0; i <= Py - 1; i++)
        for (int32 j = 0; j <= activeAnchPixs - 1; j++)
            matrix(j, Px + i) = pow(anchPixs[j].Y, i + 1);

    if (state->Parameter == ParameterNotUsed)
        for (int32 j = 0; j <= activeAnchPixs - 1; j++)
            matrix(j, activeAnchPixs - 1) = 1;

    return matrix;
}

VectorXd GetChannelVector(GradientState* state, int32 channel)
{
    int32 activeAnchPixs = state->ActiveAnchPixs;
    AnchorPixel* anchPixs = state->AnchPixs;

    VectorXd vector(activeAnchPixs);

    for (int32 i = 0; i <= activeAnchPixs - 1; i++)
        vector(i) = anchPixs[i].CH[channel];


    if (state->DiffusionIndex != DiffusionIndexNotUsed)
        for (int32 i = 0; i <= activeAnchPixs - 1; i++)
            vector(i) = Transpose((uint8)vector(i), state->DiffusionIndex);

    if (state->Parameter != ParameterNotUsed)
        for (int32 i = 0; i <= activeAnchPixs - 1; i++)
            vector(i) = vector(i) - state->Parameter;

    return vector;
}

VectorXd GetGradientQuotients(GradientState* state, MatrixXd* mainMatrix, VectorXd* channelVector)
{
    VectorXd vector(state->ActiveAnchPixs);
    vector = mainMatrix->colPivHouseholderQr().solve(*channelVector);
    return vector;
}

void GetRecurrentQuotients(GradientState* state, VectorXd* vector, VectorXd & recurrentQuotientVector_X, VectorXd & recurrentQuotientVector_Y)
{
    int32 Px = state->Px;
    int32 Py = state->Py;

    VectorXd rqvX(Px);
    VectorXd rqvY(Py);

    for (int32 i = 0; i <= Px - 1; i++)
    {
        rqvX(i) = 0;
        for (int32 j = i; j <= Px - 1; j++)
        {
            rqvX(i) = rqvX(i) + (*vector)(j) * BinomialQuotient(j + 1, i);
        }
    }

    for (int32 i = 0; i <= Py - 1; i++)
    {
        rqvY(i) = 0;
        for (int32 j = Px + i; j <= Px + Py - 1; j++)
        {
            rqvY(i) = rqvY(i) + (*vector)(j) * BinomialQuotient(j + 1 - Px, i);
        }
    }

    recurrentQuotientVector_X = rqvX;
    recurrentQuotientVector_Y = rqvY;
}

void DrawComplex(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer)
{
    double* dbl_helpBuffer = (double*)helpBuffer;

    GradientState localState = *state;
    ResampleGradientState(&localState, renderingRect);

    int32  width    = renderingRect.right  - renderingRect.left;
    int32  height   = renderingRect.bottom - renderingRect.top;

    int32  activeAnchPixs = localState.ActiveAnchPixs;
    uint8  Px             = localState.Px;
    uint8  Py             = localState.Py;
    double diff           = localState.DiffusionIndex;

    int32  padding = stride - width;
    double polyn   = 0;

    int32 pixelY = 0;
    int32 pixelX = 0;

    MatrixXd m = GetMainMatrix(&localState);
    VectorXd v = GetChannelVector(&localState, channel);
    VectorXd q = GetGradientQuotients(&localState, &m, &v);
    VectorXd qX, qY; GetRecurrentQuotients(&localState, &q, qX, qY);

    
    dbl_helpBuffer[0] = (double)q(localState.ActiveAnchPixs - 1);

    if (localState.Parameter != ParameterNotUsed)
    {
        dbl_helpBuffer[0] += (double)localState.Parameter;
    }

    if (diff == DiffusionIndexNotUsed)
    {
        if (Px >= Py)
        {
            for (int32 i = 1; i <= width - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qX, i - 1, Px - 1);
            }
            for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
            {
                *outputBuffer = ToByte(dbl_helpBuffer[pixelX]);
                outputBuffer++;
            }
            outputBuffer += padding;
            for (int32 pixelY = 1; pixelY <= height - 1; pixelY++)
            {
                polyn = Polynomial(&qY, pixelY - 1, Py - 1);
                for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
                {
                    dbl_helpBuffer[pixelX] = dbl_helpBuffer[pixelX] + polyn;
                    *outputBuffer = ToByte(dbl_helpBuffer[pixelX]);
                    outputBuffer++;
                }
                outputBuffer += padding;
            }
        }
        else
        {
            uint8* buffStore = outputBuffer;
            for (int32 i = 1; i <= height - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qY, i - 1, Py - 1);
            }
            for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
            {
                *outputBuffer = ToByte(dbl_helpBuffer[pixelY]);
                outputBuffer += stride;
            }
            for (int32 pixelX = 1; pixelX <= width - 1; pixelX++)
            {
                outputBuffer = (uint8*)(buffStore + pixelX);
                polyn = Polynomial(&qX, pixelX - 1, Px - 1);
                for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
                {
                    dbl_helpBuffer[pixelY] = dbl_helpBuffer[pixelY] + polyn;
                    *outputBuffer = ToByte(dbl_helpBuffer[pixelY]);
                    outputBuffer += stride;
                }
            }
        }
    }
    else
    {
        if (Px >= Py)
        {
            for (int32 i = 1; i <= width - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qX, i - 1, Px - 1);
            }
            for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
            {
                *outputBuffer = ToByte(UnTranspose(dbl_helpBuffer[pixelX], diff));
                outputBuffer++;
            }
            outputBuffer += padding;
            for (int32 pixelY = 1; pixelY <= height - 1; pixelY++)
            {
                polyn = Polynomial(&qY, pixelY - 1, Py - 1);
                for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
                {
                    dbl_helpBuffer[pixelX] = dbl_helpBuffer[pixelX] + polyn;
                    *outputBuffer = ToByte(UnTranspose(dbl_helpBuffer[pixelX], diff));
                    outputBuffer++;
                }
                outputBuffer += padding;
            }
        }
        else
        {
            uint8* buffStore = outputBuffer;
            for (int32 i = 1; i <= height - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qY, i - 1, Py - 1);
            }
            for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
            {
                *outputBuffer = ToByte(UnTranspose(dbl_helpBuffer[pixelY], diff));
                outputBuffer += stride;
            }
            for (int32 pixelX = 1; pixelX <= width - 1; pixelX++)
            {
                outputBuffer = (uint8*)(buffStore + pixelX);
                polyn = Polynomial(&qX, pixelX - 1, Px - 1);
                for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
                {
                    dbl_helpBuffer[pixelY] = dbl_helpBuffer[pixelY] + polyn;
                    *outputBuffer = ToByte(UnTranspose(dbl_helpBuffer[pixelY], diff));
                    outputBuffer += stride;
                }
            }
        }
    }
}

#pragma endregion


#pragma region LEGACY

void GetLinearQuotients(double L1, double L2, double c1, double c2, double & a0, double & a1)
{
    a1 = ((c1 - c2) / (L1 - L2));
    a0 = c1 - a1 * L1;
}

double LinearDistance(double x, double y, double Ax, double Ay)
{
    return Ay * y - Ax * x * LinearDistanceGranularityQuotient;
}

double RadialDistance(double x1, double y1, double x2, double y2)
{
    return pow((pow((x1 - x2), 2) + pow((y1 - y2), 2)), 0.5) * RadialDistanceGranularityQuotient;
}

double RadialDistanceSqr(double x1, double y1, double x2, double y2)
{
    return (pow((x1 - x2), 2) + pow((y1 - y2), 2)) * RadialDistanceGranularityQuotientSqr;
}

int32* ArcTanLookUp = nullptr;

void InitializeArcTanLookupTable(void)
{
    ArcTanLookUp = (int32*)malloc((AngleDistanceGranularity + 1) * sizeof(int32));
    if (!ArcTanLookUp) return;
    for (int32 p = 0; p <= AngleDistanceGranularity; p++)
    {
        ArcTanLookUp[p] = (int32)(atan(((double)p) / AngleDistanceGranularity) * AngleDistanceGranularityQuotient);
    }
}

void DestroyArcTanLookupTable(void)
{
    free(ArcTanLookUp);
}

int32 inline __fastcall Quadrant1AngleDistance(double k)
{
    if (k > 1)
    {
        k = 1 / k;
        return x05PI2546 - ArcTanLookUp[(int32)(k * AngleDistanceGranularity)];
    }
    return ArcTanLookUp[(int32)(k * AngleDistanceGranularity)];
}

double AngleDistance(double x1, double y1, double x2, double y2)
{
    double alpha = Quadrant1AngleDistance(abs((x1 - x2) / (y1 - y2)));

    if (x2 >= x1)
    {
        if   (y2 >= y1) return alpha;
        else            return PI2546 - alpha;;
    }
    else
    {
        if   (y2 >= y1) return x2PI2546 - alpha;
        else            return PI2546   + alpha;
    }
}


void PopulateLinearGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer, int32* offset)
{
    int32 activeAnchPixs  = state->ActiveAnchPixs;
    AnchorPixel* anchPixs = state->AnchPixs;

    double k = 1 / state->Angle;
    double Ay = Signum(k) / asmsqrt(1 + pow(k, 2));
    double Ax = Ay * k;

    int32  anchPixelDistance[AnchorPixelMax];
    int32  anchPixelRank    [AnchorPixelMax];

    double linearQuotient_a0[AnchorPixelMax];
    double linearQuotient_a1[AnchorPixelMax];

    for (int32 i = 0; i <= activeAnchPixs - 1; i++)
        anchPixelDistance[i] = (int32)floor(LinearDistance(anchPixs[i].X, anchPixs[i].Y, Ax, Ay));

    int32 minDistanceFromCorner =
        min(min((int32)floor(LinearDistance(0,     0,      Ax, Ay)),
                (int32)floor(LinearDistance(0,     height, Ax, Ay))),
            min((int32)floor(LinearDistance(width, 0,      Ax, Ay)),
                (int32)floor(LinearDistance(width, height, Ax, Ay))));

    int32 maxDistanceFromCorner = 
        max(max((int32)floor(LinearDistance(0,     0,      Ax, Ay)),
                (int32)floor(LinearDistance(0,     height, Ax, Ay))),
            max((int32)floor(LinearDistance(width, 0,      Ax, Ay)),
                (int32)floor(LinearDistance(width, height, Ax, Ay))));

    *offset = 
        min((int32)GetArrayMin(anchPixelDistance, activeAnchPixs), 
            minDistanceFromCorner);

    for (int32 i = 0; i <= activeAnchPixs - 1; i++)
        anchPixelDistance[i] = anchPixelDistance[i] - *offset;

    maxDistanceFromCorner = maxDistanceFromCorner - *offset;

    GetLinearRank(anchPixelDistance, anchPixelRank, activeAnchPixs);

    for (int32 i = 0; i <= activeAnchPixs - 2; i++)
    {
        int32 current = anchPixelRank[i];
        int32 next    = anchPixelRank[i + 1];

        GetLinearQuotients(anchPixelDistance[current], 
                           anchPixelDistance[next], 
                           anchPixs[current].CH[channel], 
                           anchPixs[next].CH[channel], 
                           linearQuotient_a0[current], 
                           linearQuotient_a1[current]);
    }

    int32 leftDistance = anchPixelDistance[anchPixelRank[0]];
    uint8 leftColor    = anchPixs[anchPixelRank[0]].CH[channel];

    for (int32 i = 0; i < leftDistance; i++) lookupBuffer[i] = leftColor;

    double q1, q0, result = 0;

    for (int32 c = 0; c <= activeAnchPixs - 2; c++)
    {
        lookupBuffer[anchPixelDistance[anchPixelRank[c]]] = anchPixs[anchPixelRank[c]].CH[channel];

        q1 = (4 * linearQuotient_a1[anchPixelRank[c]] * state->Grad) / (anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]);
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c]] - anchPixelDistance[anchPixelRank[c + 1]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c]] + 1; i < (int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) + 1; i++)
        {
            result = result + q1 * i + q0;
            lookupBuffer[i] = (uint8)result;
        }

        q1 = -q1;
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c + 1]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c + 1]] - 1; i > (int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) - 1; i--)
        {
            result = result - q1 * i - q0;
            lookupBuffer[i] = (uint8)result;
        }
    }

    int32 rightDistance = anchPixelDistance[anchPixelRank[activeAnchPixs - 1]];
    uint8 rightColor    = lookupBuffer[rightDistance] = anchPixs[anchPixelRank[activeAnchPixs - 1]].CH[channel];

    for (int32 i = rightDistance; i <= maxDistanceFromCorner + 1; i++) lookupBuffer[i] = rightColor;
}

void PopulateRadialGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer)
{
    int32 cX = state->AnchPixs[state->CentralPix].X;
    int32 cY = state->AnchPixs[state->CentralPix].Y;

    int32 activeAnchPixs  = state->ActiveAnchPixs;
    AnchorPixel* anchPixs = state->AnchPixs;

    int32  anchPixelDistance[AnchorPixelMax];
    int32  anchPixelRank    [AnchorPixelMax];

    double linearQuotient_a0[AnchorPixelMax];
    double linearQuotient_a1[AnchorPixelMax];

    for (int32 i = 0; i <= activeAnchPixs - 1; i++)
        anchPixelDistance[i] = (int32)floor(RadialDistance(cX, cY, anchPixs[i].X, anchPixs[i].Y));

    int32 maxDistanceFromCorner = 
        max(max((int32)floor(RadialDistance(cX, cY, 0, 0)),
                (int32)floor(RadialDistance(cX, cY, 0, height))),
            max((int32)floor(RadialDistance(cX, cY, width, 0)),
                (int32)floor(RadialDistance(cX, cY, width, height))));

    GetLinearRank(anchPixelDistance, anchPixelRank, activeAnchPixs);

    for (int32 i = 0; i <= activeAnchPixs - 2; i++)
    {
        int32 current = anchPixelRank[i];
        int32 next    = anchPixelRank[i + 1];

        GetLinearQuotients(anchPixelDistance[current], 
                           anchPixelDistance[next], 
                           anchPixs[current].CH[channel], 
                           anchPixs[next].CH[channel], 
                           linearQuotient_a0[current], 
                           linearQuotient_a1[current]);
    }

    double q1, q0, result = 0;

    for (int32 c = 0; c <= activeAnchPixs - 2; c++)
    {
        lookupBuffer[anchPixelDistance[anchPixelRank[c]]] = anchPixs[anchPixelRank[c]].CH[channel];

        q1 = (4 * linearQuotient_a1[anchPixelRank[c]] * state->Grad) / (anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]);
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c]] - anchPixelDistance[anchPixelRank[c + 1]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c]] + 1; i < (int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) + 1; i++)
        {
            result = result + q1 * i + q0;
            lookupBuffer[i] = (uint8)result;
        }

        q1 = -q1;
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c + 1]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c + 1]] - 1; i >(int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) - 1; i--)
        {
            result = result - q1 * i - q0;
            lookupBuffer[i] = (uint8)result;
        }
    }

    uint8 rightColor = lookupBuffer[anchPixelDistance[anchPixelRank[activeAnchPixs - 1]]] = anchPixs[anchPixelRank[activeAnchPixs - 1]].CH[channel];

    if (maxDistanceFromCorner > anchPixelDistance[anchPixelRank[activeAnchPixs - 1]])
        for (int32 i = anchPixelDistance[anchPixelRank[activeAnchPixs - 1]]; i <= maxDistanceFromCorner + 1; i++)
            lookupBuffer[i] = rightColor;

}

void PopulateAngledGradientLookup(GradientState* state, int32 width, int32 height, int32 channel, uint8* lookupBuffer)
{
    int32 cX = state->CenterX;
    int32 cY = state->CenterY;

    int32  activeAnchPixs = state->ActiveAnchPixs;
    AnchorPixel* anchPixs = state->AnchPixs;

    int32  anchPixelDistance[AnchorPixelMax];
    int32  anchPixelRank    [AnchorPixelMax];

    double linearQuotient_a0[AnchorPixelMax];
    double linearQuotient_a1[AnchorPixelMax];

    for (int32 i = 0; i <= activeAnchPixs - 1; i++)
        anchPixelDistance[i] = (int32)floor(AngleDistance(cX, cY, anchPixs[i].X, anchPixs[i].Y));

    GetLinearRank(anchPixelDistance, anchPixelRank, activeAnchPixs);

    int32 current, next;

    for (int32 i = 0; i <= activeAnchPixs - 2; i++)
    {
        current = anchPixelRank[i];
        next    = anchPixelRank[i + 1];

        GetLinearQuotients(anchPixelDistance[current], 
                           anchPixelDistance[next], 
                           anchPixs[current].CH[channel], 
                           anchPixs[next].CH[channel], 
                           linearQuotient_a0[current], 
                           linearQuotient_a1[current]);
    }

    current = anchPixelRank[activeAnchPixs - 1];
    next    = anchPixelRank[0];

    GetLinearQuotients(anchPixelDistance[current], 
                       anchPixelDistance[next] + x2PI2546, 
                       anchPixs[current].CH[channel], 
                       anchPixs[next].CH[channel], 
                       linearQuotient_a0[current], 
                       linearQuotient_a1[current]);

    double q1, q0, result = 0;

    for (int32 c = 0; c <= activeAnchPixs - 2; c++)
    {
        lookupBuffer[anchPixelDistance[anchPixelRank[c]]] = anchPixs[anchPixelRank[c]].CH[channel];

        q1 = (4 * linearQuotient_a1[anchPixelRank[c]] * state->Grad) / (anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]);
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c]] - anchPixelDistance[anchPixelRank[c + 1]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c]] + 1; i < (int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) + 1; i++)
        {
            result = result + q1 * i + q0;
            lookupBuffer[i] = (uint8)result;
        }

        q1 = -q1;
        q0 = q1 * (2 - 3 * anchPixelDistance[anchPixelRank[c + 1]] - anchPixelDistance[anchPixelRank[c]]) / 4 + linearQuotient_a1[anchPixelRank[c]];
        result = anchPixs[anchPixelRank[c + 1]].CH[channel];

        for (int32 i = anchPixelDistance[anchPixelRank[c + 1]] - 1; i >(int32)((anchPixelDistance[anchPixelRank[c + 1]] + anchPixelDistance[anchPixelRank[c]]) / 2) - 1; i--)
        {
            result = result - q1 * i - q0;
            lookupBuffer[i] = (uint8)result;
        }
    }

    int32 rightDistance = anchPixelDistance[anchPixelRank[activeAnchPixs - 1]];
    
    for (int32 i = rightDistance; i <= x2PI2546; i++)
        lookupBuffer[i] = (uint8)(linearQuotient_a1[current] * i + linearQuotient_a0[current]);

    for (int32 i = 0; i < anchPixelDistance[anchPixelRank[0]]; i++)
        lookupBuffer[i] = (uint8)(linearQuotient_a1[current] * (i + x2PI2546) + linearQuotient_a0[current]);

}

void DrawLinear(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer)
{
    GradientState localState = *state;
    ResampleGradientState(&localState, renderingRect);

    int32 width  = renderingRect.right - renderingRect.left;
    int32 height = renderingRect.bottom - renderingRect.top;

    int32 offset = 0;

    PopulateLinearGradientLookup(&localState, width, height, channel, helpBuffer, &offset);

    double k = 1 / localState.Angle;
    double Ay = Signum(k) / asmsqrt(1 + pow(k, 2));
    double Ax = Ay * k;
    double distance = 0;

    int32  padding = stride - width;

    for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
    {
        distance = Ay * pixelY - offset;
        for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
        {
            *outputBuffer = helpBuffer[(int32)distance];
            outputBuffer++;
            distance -= Ax;
        }
        outputBuffer += padding;
    }
}

void DrawRadial(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer)
{
    GradientState localState = *state;
    ResampleGradientState(&localState, renderingRect);

    int32 width  = renderingRect.right  - renderingRect.left;
    int32 height = renderingRect.bottom - renderingRect.top;

    PopulateRadialGradientLookup(&localState, width, height, channel, helpBuffer);

    int32 cX = localState.AnchPixs[localState.CentralPix].X;
    int32 cY = localState.AnchPixs[localState.CentralPix].Y;

    int32 padding = stride - width;
    int32 radius = 0;
    double radiussqr;
    double r = 9 - 18 * cX;

    for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
    {
        radiussqr = RadialDistanceSqr(cX, cY, 0, pixelY);
        for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
        {
            radius = (int32)(asmsqrt(radiussqr));
            *outputBuffer = helpBuffer[radius];
            outputBuffer++;
            radiussqr = radiussqr + 18 * pixelX + r;
        }
        outputBuffer += padding;
    }
}

void DrawAngled(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer)
{
    GradientState localState = *state;
    ResampleGradientState(&localState, renderingRect);

    int32 width  = renderingRect.right  - renderingRect.left;
    int32 height = renderingRect.bottom - renderingRect.top;

    PopulateAngledGradientLookup(&localState, width, height, channel, helpBuffer);

    int32 cX = localState.CenterX;
    int32 cY = localState.CenterY;

    int32 Ys, Yp, Ye;
    int32 Xs, Xp, Xe;

    Xs = 0;
    Xe = width - 1;

    if      (cX < 0)         Xp = -1;
    else if (cX > width - 1) Xp = width;
    else                     Xp = cX;

    Ys = 0;
    Ye = height - 1;

    if      (cY < 0)          Yp = -1;
    else if (cY > height - 1) Yp = height;
    else                      Yp = cY;


    double A, B;
    uint8* buff;
    int32 padding;

    buff = outputBuffer + (int32)(stride * Ys + Xs);
    padding = stride - Xp + Xs;
    for (int32 pixelY = Ys; pixelY <= Yp - 1; pixelY++)
    {
        A = (double)cX / (double)(cY - pixelY);
        B = (double)1 / (double)(cY - pixelY);
        for (int32 pixelX = Xp - 1; pixelX >= Xs; pixelX--)
        {
            *buff = helpBuffer[PI2546 + Quadrant1AngleDistance(A)];
            A -= B;
            buff++;
        }
        buff += padding;
    }

    buff = outputBuffer + (int32)(stride * (Yp + 1) + Xs);
    padding = stride - Xp + Xs;
    for (int32 pixelY = Yp + 1; pixelY <= Ye; pixelY++)
    {
        A = (double)cX / (double)(pixelY - cY);
        B = (double)1 / (double)(pixelY - cY);
        for (int32 pixelX = Xp - 1; pixelX >= Xs; pixelX--)
        {
            *buff = helpBuffer[x2PI2546 - Quadrant1AngleDistance(A)];
            A -= B;
            buff++;
        }
        buff += padding;
    }

    buff = outputBuffer + (int32)(stride * (Yp + 1) + (Xp + 1));
    padding = stride - Xe + Xp;
    for (int32 pixelY = Yp + 1; pixelY <= Ye; pixelY++)
    {

        A = (double)(Xp + 1 - cX) / (double)(-cY + pixelY);
        B = (double)1 / (double)(-cY + pixelY);
        for (int32 pixelX = Xp + 1; pixelX <= Xe; pixelX++)
        {
            *buff = helpBuffer[Quadrant1AngleDistance(A)];
            A += B;
            buff++;
        }
        buff += padding;
    }

    buff = outputBuffer + (int32)(stride * Ys + (Xp + 1));
    padding = stride - Xe + Xp;
    for (int32 pixelY = Ys; pixelY <= Yp - 1; pixelY++)
    {

        A = (double)(Xp + 1 - cX) / (double)(cY - pixelY);
        B = (double)1 / (double)(cY - pixelY);
        for (int32 pixelX = Xp + 1; pixelX <= Xe; pixelX++)
        {
            *buff = helpBuffer[PI2546 - Quadrant1AngleDistance(A)];
            A += B;
            buff++;
        }
        buff += padding;
    }

    if (Yp != -1 && Yp != height)
    {
        buff = outputBuffer + (int32)(stride * cY);
        for (int32 pixelX = Xs; pixelX < Xp; pixelX++)
        {
            *buff = helpBuffer[PI2546 + x05PI2546];
            buff++;
        }
        buff++;
        for (int32 pixelX = Xp + 1; pixelX < Xe; pixelX++)
        {
            *buff = helpBuffer[x05PI2546];
            buff++;
        }
    }

    if (Xp != -1 && Xp != width)
    {
        buff = outputBuffer + Xp;
        for (int32 pixelY = Ys; pixelY < Yp; pixelY++)
        {
            *buff = helpBuffer[PI2546];
            buff += stride;
        }
        for (int32 pixelY = Yp; pixelY < Ye; pixelY++)
        {
            *buff = helpBuffer[0];
            buff += stride;
        }
    }
}

#pragma endregion


void DrawGradient(GradientState* state, VRect renderingRect, int32 stride, int32 channel, uint8* helpBuffer, uint8* outputBuffer)
{
    switch (state->Type)
    {
    case GradientType::COMPLEX:
    {
        DrawComplex(state, renderingRect, stride, channel, helpBuffer, outputBuffer);
        break;
    }
    case GradientType::LINEAR:
    {
        DrawLinear(state, renderingRect, stride, channel, helpBuffer, outputBuffer);
        break;
    }
    case GradientType::RADIAL:
    {
        DrawRadial(state, renderingRect, stride, channel, helpBuffer, outputBuffer);
        break;
    }
    case GradientType::ANGLED:
    {
        DrawAngled(state, renderingRect, stride, channel, helpBuffer, outputBuffer);
        break;
    }
    default:
        break;
    }
}


#pragma region PROXY

void GetRectangles(ProxyState* proxyState, RectangleRecord* rects)
{

    double zoom             = proxyState->ZoomFactor;
    VPoint viewportLocation = proxyState->ViewportLocation;
    VPoint viewportSize     = proxyState->ViewportSize;

    VPoint imageSize;
    imageSize.h = gFilterRecord->documentInfo->bounds.right;
    imageSize.v = gFilterRecord->documentInfo->bounds.bottom;


    rects->ReadRectangle.top  = viewportLocation.v;
    rects->ReadRectangle.left = viewportLocation.h;
    rects->ReadRectangle.bottom = rects->ReadRectangle.top  + (int32)((double)viewportSize.v / zoom);
    rects->ReadRectangle.right  = rects->ReadRectangle.left + (int32)((double)viewportSize.h / zoom);


    if (rects->ReadRectangle.top    < 0) rects->ReadRectangle.top    = 0;
    if (rects->ReadRectangle.left   < 0) rects->ReadRectangle.left   = 0;
    if (rects->ReadRectangle.bottom < 0) rects->ReadRectangle.bottom = 0;
    if (rects->ReadRectangle.right  < 0) rects->ReadRectangle.right  = 0;
    if (rects->ReadRectangle.top    > imageSize.v) rects->ReadRectangle.top    = imageSize.v;
    if (rects->ReadRectangle.left   > imageSize.h) rects->ReadRectangle.left   = imageSize.h;
    if (rects->ReadRectangle.bottom > imageSize.v) rects->ReadRectangle.bottom = imageSize.v;
    if (rects->ReadRectangle.right  > imageSize.h) rects->ReadRectangle.right  = imageSize.h;

    rects->WriteRectangle.top    = 0;
    rects->WriteRectangle.left   = 0;
    rects->WriteRectangle.bottom = (int32)((double)(rects->ReadRectangle.bottom - rects->ReadRectangle.top) * zoom);
    rects->WriteRectangle.right  = (int32)((double)(rects->ReadRectangle.right - rects->ReadRectangle.left) * zoom);


    VRect absRenderingRect = gFilterRecord->bigDocumentData->filterRect32;

    if (absRenderingRect.top    < rects->ReadRectangle.top)    
        absRenderingRect.top    = rects->ReadRectangle.top;
    if (absRenderingRect.left   < rects->ReadRectangle.left)   
        absRenderingRect.left   = rects->ReadRectangle.left;
    if (absRenderingRect.bottom < rects->ReadRectangle.top)    
        absRenderingRect.bottom = rects->ReadRectangle.top;
    if (absRenderingRect.right  < rects->ReadRectangle.left)   
        absRenderingRect.right  = rects->ReadRectangle.left;
    if (absRenderingRect.top    > rects->ReadRectangle.bottom) 
        absRenderingRect.top    = rects->ReadRectangle.bottom;
    if (absRenderingRect.left   > rects->ReadRectangle.right)  
        absRenderingRect.left   = rects->ReadRectangle.right;
    if (absRenderingRect.bottom > rects->ReadRectangle.bottom) 
        absRenderingRect.bottom = rects->ReadRectangle.bottom;
    if (absRenderingRect.right  > rects->ReadRectangle.right)  
        absRenderingRect.right  = rects->ReadRectangle.right;

    rects->SelectionRectangle = absRenderingRect;


    absRenderingRect.top    = (int32)(zoom * absRenderingRect.top);
    absRenderingRect.left   = (int32)(zoom * absRenderingRect.left);
    absRenderingRect.bottom = (int32)(zoom * absRenderingRect.bottom);
    absRenderingRect.right  = (int32)(zoom * absRenderingRect.right);

    rects->RenderingRectangle = absRenderingRect;


    absRenderingRect.top    = absRenderingRect.top    - (int32)(zoom * rects->ReadRectangle.top);
    absRenderingRect.left   = absRenderingRect.left   - (int32)(zoom * rects->ReadRectangle.left);
    absRenderingRect.bottom = absRenderingRect.bottom - (int32)(zoom * rects->ReadRectangle.top);
    absRenderingRect.right  = absRenderingRect.right  - (int32)(zoom * rects->ReadRectangle.left);

    rects->WriteRenderingRectangle = absRenderingRect;
}

void WriteRectangle(uint8* destination, VRect writeRectangle, int32 pixelSize, int32 offset, int32 stride, uint8 color)
{
    uint8* buffer = destination + offset;
    int32 padding = stride - pixelSize * writeRectangle.right;
    int32 i, j;
    for (j = 0; j < writeRectangle.bottom; j++)
    {
        for (i = 0; i < writeRectangle.right; i++)
        {
            *buffer = color;
            buffer += pixelSize;
        }
        buffer += padding;
    }
}

void WriteProxyMergedChannels(RectangleRecord* rects, int32 proxyPixelSize, int32 stride, uint8* destination)
{
    PSScaling scaling;
    scaling.destinationRect = rects->WriteRectangle;
    scaling.sourceRect      = rects->ReadRectangle;

    PixelMemoryDesc memoryDescriptor;
    memoryDescriptor.data      = (void*)destination;
    memoryDescriptor.rowBits   = 8 * stride;
    memoryDescriptor.colBits   = 8 * proxyPixelSize ;
    memoryDescriptor.depth     = 8;
    memoryDescriptor.bitOffset = 0;

    ReadChannelDesc* channelDescriptor;
    
    channelDescriptor = gFilterRecord->documentInfo->mergedTransparency;

    if (channelDescriptor)
    {
        memoryDescriptor.data = (void*)(destination + transparencyChannel);
        gData->sPSChannelPortsSuite->ReadScaledPixels(channelDescriptor->port, &(rects->WriteRectangle), &scaling, &memoryDescriptor);
    }
    else
    {
        WriteRectangle(destination, rects->WriteRectangle, proxyPixelSize, transparencyChannel, stride, 255);
    }

    channelDescriptor = gFilterRecord->documentInfo->mergedCompositeChannels;

    for (int32 channel = 0; channel <= 2; channel++)
    {
        memoryDescriptor.data = (void*)(destination + 2 - channel);
        gData->sPSChannelPortsSuite->ReadScaledPixels(channelDescriptor->port, &(rects->WriteRectangle), &scaling, &memoryDescriptor);
        channelDescriptor = channelDescriptor->next;
    }
}

void(*WriteByte)(uint8 b, uint8* address);

void OverwriteByte(uint8 b, uint8* address)
{
    *address = b;
}

void SuperimposeByte(uint8 b, uint8* address)
{
    if (*address > b) 
        *address = b;
}

void WriteProxyChannelComplex(GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination)
{
    double* dbl_helpBuffer = (double*)helpBuffer;

    if  (superimpose) WriteByte = &SuperimposeByte;
    else              WriteByte = &OverwriteByte;

    int32 width  = rects->RenderingRectangle.right  - rects->RenderingRectangle.left;
    int32 height = rects->RenderingRectangle.bottom - rects->RenderingRectangle.top;

    int32 activeAnchPixs = grState->ActiveAnchPixs;
    uint8 Px             = grState->Px;
    uint8 Py             = grState->Py;
    double diff          = grState->DiffusionIndex;
    
    uint8* proxyBuffer = destination + stride * rects->WriteRenderingRectangle.top + proxyPixelSize * rects->WriteRenderingRectangle.left + bufferOffset;
    int32  padding = stride - proxyPixelSize * width;
    double polyn = 0;

    int32 pixelX = 0;
    int32 pixelY = 0;

    MatrixXd m = GetMainMatrix(grState);
    VectorXd v = GetChannelVector(grState, channel);
    VectorXd q = GetGradientQuotients(grState, &m, &v);
    VectorXd qX, qY; GetRecurrentQuotients(grState, &q, qX, qY);

    dbl_helpBuffer[0] = (double)q(activeAnchPixs - 1);

    if (grState->Parameter != ParameterNotUsed)
    {
        dbl_helpBuffer[0] = dbl_helpBuffer[0] + (double)grState->Parameter;
    }
    for (int32 i = 1; i <= width - 1; i++)
    {
        dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qX, i - 1, Px - 1);
    }
    for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
    {
        WriteByte(ToByte(dbl_helpBuffer[pixelX]), proxyBuffer);
        proxyBuffer += proxyPixelSize;
    }
    proxyBuffer += padding;

    if (diff == DiffusionIndexNotUsed)
    {
        if (Px >= Py)
        {
            for (int32 i = 1; i <= width - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qX, i - 1, Px - 1);
            }
            for (pixelX = 0; pixelX <= width - 1; pixelX++)
            {
                WriteByte(ToByte(dbl_helpBuffer[pixelX]), proxyBuffer);
                proxyBuffer += proxyPixelSize;
            }
            proxyBuffer += padding;
            for (pixelY = 1; pixelY <= height - 1; pixelY++)
            {
                polyn = Polynomial(&qY, pixelY - 1, Py - 1);
                for (pixelX = 0; pixelX <= width - 1; pixelX++)
                {
                    dbl_helpBuffer[pixelX] = dbl_helpBuffer[pixelX] + polyn;
                    WriteByte(ToByte(dbl_helpBuffer[pixelX]), proxyBuffer);
                    proxyBuffer += proxyPixelSize;
                }
                proxyBuffer += padding;
            }
        }
        else
        {
            uint8* buffStore = proxyBuffer;
            for (int32 i = 1; i <= height - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qY, i - 1, Py - 1);
            }
            for (pixelY = 0; pixelY <= height - 1; pixelY++)
            {
                WriteByte(ToByte(dbl_helpBuffer[pixelY]), proxyBuffer);
                proxyBuffer += stride;
            }
            for (pixelX = 1; pixelX <= width - 1; pixelX++)
            {
                proxyBuffer = (uint8*)(buffStore + proxyPixelSize * pixelX);
                polyn = Polynomial(&qX, pixelX - 1, Px - 1);
                for (pixelY = 0; pixelY <= height - 1; pixelY++)
                {
                    dbl_helpBuffer[pixelY] = dbl_helpBuffer[pixelY] + polyn;
                    WriteByte(ToByte(dbl_helpBuffer[pixelY]), proxyBuffer);
                    proxyBuffer += stride;
                }
            }
        }
    }
    else
    {
        if (Px >= Py)
        {
            for (int32 i = 1; i <= width - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qX, i - 1, Px - 1);
            }
            for (pixelX = 0; pixelX <= width - 1; pixelX++)
            {
                WriteByte(ToByte(UnTranspose(dbl_helpBuffer[pixelX], diff)), proxyBuffer);
                proxyBuffer += proxyPixelSize;
            }
            proxyBuffer += padding;
            for (pixelY = 1; pixelY <= height - 1; pixelY++)
            {
                polyn = Polynomial(&qY, pixelY - 1, Py - 1);
                for (pixelX = 0; pixelX <= width - 1; pixelX++)
                {
                    dbl_helpBuffer[pixelX] = dbl_helpBuffer[pixelX] + polyn;
                    WriteByte(ToByte(UnTranspose(dbl_helpBuffer[pixelX], diff)), proxyBuffer);
                    proxyBuffer += proxyPixelSize;
                }
                proxyBuffer += padding;
            }
        }
        else
        {
            uint8* buffStore = proxyBuffer;
            for (int32 i = 1; i <= height - 1; i++)
            {
                dbl_helpBuffer[i] = dbl_helpBuffer[i - 1] + Polynomial(&qY, i - 1, Py - 1);
            }
            for (pixelY = 0; pixelY <= height - 1; pixelY++)
            {
                WriteByte(ToByte(UnTranspose(dbl_helpBuffer[pixelY], diff)), proxyBuffer);
                proxyBuffer += stride;
            }
            for (pixelX = 1; pixelX <= width - 1; pixelX++)
            {
                proxyBuffer = (uint8*)(buffStore + proxyPixelSize * pixelX);
                polyn = Polynomial(&qX, pixelX - 1, Px - 1);
                for (pixelY = 0; pixelY <= height - 1; pixelY++)
                {
                    dbl_helpBuffer[pixelY] = dbl_helpBuffer[pixelY] + polyn;
                    WriteByte(ToByte(UnTranspose(dbl_helpBuffer[pixelY], diff)), proxyBuffer);
                    proxyBuffer += stride;
                }
            }
        }
    }
}

void WriteProxyChannelLinear(GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination)
{

    if  (superimpose) WriteByte = &SuperimposeByte;
    else              WriteByte = &OverwriteByte;

    int32 width  = rects->RenderingRectangle.right  - rects->RenderingRectangle.left;
    int32 height = rects->RenderingRectangle.bottom - rects->RenderingRectangle.top;

    int32 offset = 0;   

    PopulateLinearGradientLookup(grState, width, height, channel, helpBuffer, &offset);

    double k = 1 / grState->Angle;
    double Ay = Signum(k) / sqrt(1 + pow(k, 2));
    double Ax = Ay * k;
    double distance = 0;

    uint8* proxyBuffer = destination + stride * rects->WriteRenderingRectangle.top + proxyPixelSize * rects->WriteRenderingRectangle.left + bufferOffset;
    int32  padding = stride - proxyPixelSize * width;

    for (int32 pixelY = 0; pixelY <= height - 1; pixelY++)
    {
        distance = Ay * pixelY - offset;
        for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
        {
            WriteByte(helpBuffer[(int32)distance], proxyBuffer);
            proxyBuffer += proxyPixelSize;
            distance -= Ax;
        }
        proxyBuffer += padding;
    }

}

void WriteProxyChannelRadial(GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination)
{

    if  (superimpose) WriteByte = &SuperimposeByte;
    else              WriteByte = &OverwriteByte;

    int32 width  = rects->RenderingRectangle.right  - rects->RenderingRectangle.left;
    int32 height = rects->RenderingRectangle.bottom - rects->RenderingRectangle.top; 
    if (width  % 2 == 0) width  += 1;
    if (height % 2 == 0) height += 1;

    PopulateRadialGradientLookup(grState, width, height, channel, helpBuffer);

    uint8* proxyBuffer = destination + stride * rects->WriteRenderingRectangle.top + proxyPixelSize * rects->WriteRenderingRectangle.left + bufferOffset;
    int32 padding = 2 * stride - proxyPixelSize * width - proxyPixelSize;
    int32 cX = grState->AnchPixs[grState->CentralPix].X;
    int32 cY = grState->AnchPixs[grState->CentralPix].Y;
    
    int32  radius    = 0;
    double radiussqr = 0;

    uint8* outputAddress        = proxyBuffer;
    uint8* interpolationInput_a = nullptr;
    uint8* interpolationInput_b = nullptr;

    double nj = 36 - 36 * cX;

    for (int32 pixelY = 0; pixelY <= height - 1; pixelY += 2)
    {
        radiussqr = RadialDistanceSqr(cX, cY, 0, pixelY);
        for (int32 pixelX = 0; pixelX <= width - 1; pixelX += 2)
        {
            radius = (int32)(asmsqrt(radiussqr));
            WriteByte(helpBuffer[radius], outputAddress);
            outputAddress += 2 * proxyPixelSize;
            radiussqr = radiussqr + 36 * pixelX + nj;
        }
        outputAddress += padding;
    }

    outputAddress        = proxyBuffer + proxyPixelSize;
    interpolationInput_a = proxyBuffer;
    interpolationInput_b = proxyBuffer + 2 * proxyPixelSize;

    for (int32 pixelY = 0; pixelY <= height - 1; pixelY += 2)
    {
        for (int32 pixelX = 1; pixelX <= width - 2; pixelX += 2)
        {
            *outputAddress = (*interpolationInput_a + *interpolationInput_b) / 2;

            outputAddress        += 2 * proxyPixelSize;
            interpolationInput_a += 2 * proxyPixelSize;
            interpolationInput_b += 2 * proxyPixelSize;
        }
        outputAddress        += padding + 2 * proxyPixelSize;
        interpolationInput_a += padding + 2 * proxyPixelSize;
        interpolationInput_b += padding + 2 * proxyPixelSize;
    }

    outputAddress        = proxyBuffer + stride;
    interpolationInput_a = proxyBuffer;
    interpolationInput_b = proxyBuffer + 2 * stride;

    for (int32 pixelY = 1; pixelY <= height - 2; pixelY += 2)
    {
        for (int32 pixelX = 0; pixelX <= width - 1; pixelX++)
        {
            *outputAddress = (*interpolationInput_a + *interpolationInput_b) / 2;

            outputAddress        += proxyPixelSize;
            interpolationInput_a += proxyPixelSize;
            interpolationInput_b += proxyPixelSize;
        }
        outputAddress        += padding + proxyPixelSize;
        interpolationInput_a += padding + proxyPixelSize;
        interpolationInput_b += padding + proxyPixelSize;
    }

}

void WriteProxyChannelAngled(GradientState* grState, RectangleRecord* rects, uint8* helpBuffer, bool superimpose, int32 channel, int32 proxyPixelSize, int32 bufferOffset, int32 stride, uint8* destination)
{

    if  (superimpose) WriteByte = &SuperimposeByte;
    else              WriteByte = &OverwriteByte;

    int32 width =  rects->RenderingRectangle.right  - rects->RenderingRectangle.left;
    int32 height = rects->RenderingRectangle.bottom - rects->RenderingRectangle.top;
    if (height % 2 == 0) height += 1;
    if (width  % 2 == 0) width  += 1;


    PopulateAngledGradientLookup(grState, width, height, channel, helpBuffer);


    int32 cX = grState->CenterX;
    int32 cY = grState->CenterY;

    int32 Ys, Yp, Ye;
    int32 Xs, Xp, Xe;

    Xs = 0;
    Xe = width - 1;

    if      (cX < 0)         Xp = -1;
    else if (cX > width - 1) Xp = width;
    else                     Xp = cX;

    Ys = 0;
    Ye = height - 1;

    if      (cY < 0)          Yp = -1;
    else if (cY > height - 1) Yp = height;
    else                      Yp = cY;

    uint8* proxyBuffer = destination + stride * rects->WriteRenderingRectangle.top + proxyPixelSize * rects->WriteRenderingRectangle.left + bufferOffset;
    uint8* outputAddress = proxyBuffer;
    int32 padding = 0;

    double A, B;

    outputAddress = proxyBuffer + (int32)(stride * Ys + Xs* proxyPixelSize);
    padding = stride + (-Xp + Xs) * proxyPixelSize;
    for (int32 pixelY = Ys; pixelY <= Yp - 1; pixelY++)
    {
        A = (double)cX / (double)(cY - pixelY);
        B = (double)1 / (double)(cY - pixelY);
        for (int32 pixelX = Xp - 1; pixelX >= Xs; pixelX--)
        {
            WriteByte(helpBuffer[PI2546 + Quadrant1AngleDistance(A)], outputAddress);
            A -= B;
            outputAddress += proxyPixelSize;
        }
        outputAddress += padding;
    }

    outputAddress = proxyBuffer + (int32)(stride * (Yp + 1) + Xs* proxyPixelSize);
    padding = stride + (-Xp + Xs) * proxyPixelSize;
    for (int32 pixelY = Yp + 1; pixelY <= Ye; pixelY++)
    {
        A = (double)cX / (double)(pixelY - cY);
        B = (double)1 / (double)(pixelY - cY);
        for (int32 pixelX = Xp - 1; pixelX >= Xs; pixelX--)
        {
            WriteByte(helpBuffer[x2PI2546 - Quadrant1AngleDistance(A)], outputAddress);
            A -= B;
            outputAddress += proxyPixelSize;
        }
        outputAddress += padding;
    }

    outputAddress = proxyBuffer + (int32)(stride * (Yp + 1) + (Xp + 1)* proxyPixelSize);
    padding = stride + (-Xe + Xp) * proxyPixelSize;
    for (int32 pixelY = Yp + 1; pixelY <= Ye; pixelY++)
    {

        A = (double)(Xp + 1 - cX) / (double)(-cY + pixelY);
        B = (double)1 / (double)(-cY + pixelY);
        for (int32 pixelX = Xp + 1; pixelX <= Xe; pixelX++)
        {
            WriteByte(helpBuffer[Quadrant1AngleDistance(A)], outputAddress);
            A += B;
            outputAddress += proxyPixelSize;
        }
        outputAddress += padding;
    }

    outputAddress = proxyBuffer + (int32)(stride * Ys + (Xp + 1)* proxyPixelSize);
    padding = stride + (-Xe + Xp) * proxyPixelSize;
    for (int32 pixelY = Ys; pixelY <= Yp - 1; pixelY++)
    {

        A = (double)(Xp + 1 - cX) / (double)(cY - pixelY);
        B = (double)1 / (double)(cY - pixelY);
        for (int32 pixelX = Xp + 1; pixelX <= Xe; pixelX++)
        {
            WriteByte(helpBuffer[PI2546 - Quadrant1AngleDistance(A)], outputAddress);
            A += B;
            outputAddress += proxyPixelSize;
        }
        outputAddress += padding;
    }
    if (Yp != -1 && Yp != height)
    {
        outputAddress = proxyBuffer + (int32)(stride * cY);
        for (int32 pixelX = Xs; pixelX < Xp; pixelX++)
        {
            WriteByte(helpBuffer[PI2546 + x05PI2546], outputAddress);
            outputAddress += proxyPixelSize;
        }
        outputAddress += proxyPixelSize;
        for (int32 pixelX = Xp + 1; pixelX < Xe; pixelX++)
        {
            WriteByte(helpBuffer[x05PI2546], outputAddress);
            outputAddress += proxyPixelSize;
        }
    }
    if (Xp != -1 && Xp != width)
    {
        outputAddress = proxyBuffer + Xp* proxyPixelSize;
        for (int32 pixelY = Ys; pixelY < Yp; pixelY++)
        { 
            WriteByte(helpBuffer[PI2546], outputAddress);
            outputAddress += stride;
        }
        for (int32 pixelY = Yp; pixelY < Ye; pixelY++)
        {
            WriteByte(helpBuffer[0], outputAddress);
            outputAddress += stride;
        }
    }
}

void WriteProxyGradient(GradientState* grState, ProxyState* pState, RectangleRecord* rects, uint8* helpBuffer, bool pStateChanged, int32 proxyPixelSize, int32 stride, uint8* destination)
{
    WriteProxyChannelGradientFunction writeProxyChannelGradient;

    switch (grState->Type)
    {
        case COMPLEX: writeProxyChannelGradient = &WriteProxyChannelComplex; break;
        case LINEAR:  writeProxyChannelGradient = &WriteProxyChannelLinear;  break;
        case RADIAL:  writeProxyChannelGradient = &WriteProxyChannelRadial;  break;
        case ANGLED:  writeProxyChannelGradient = &WriteProxyChannelAngled;  break;
        default:
            return;
    }

    GradientState localState = *grState;
    ResampleGradientState(&localState, rects->RenderingRectangle, pState->ZoomFactor);

    for (int32 channel = 0; channel <= 2; channel++)
        writeProxyChannelGradient(&localState, rects, helpBuffer, false, channel, proxyPixelSize, 2 - channel, stride, destination);

    PSScaling scaling;
    scaling.destinationRect = rects->WriteRenderingRectangle;
    scaling.sourceRect      = rects->SelectionRectangle;

    uint8* buffer = destination + stride * rects->WriteRenderingRectangle.top + proxyPixelSize * rects->WriteRenderingRectangle.left;

    PixelMemoryDesc memoryDescriptor;
    memoryDescriptor.data      = (void*)buffer;
    memoryDescriptor.rowBits   = 8 * stride;
    memoryDescriptor.colBits   = 8 * proxyPixelSize;
    memoryDescriptor.depth     = 8;
    memoryDescriptor.bitOffset = 8 * transparencyChannel;

    ReadChannelDesc* channelDescriptor = gFilterRecord->documentInfo->selection;

    if (grState->OpacityApplied)
    {
        if (pStateChanged) WriteRectangle(destination, rects->WriteRectangle, proxyPixelSize, transparencyChannel, stride, 0);
        if (channelDescriptor)
        {
            gData->sPSChannelPortsSuite->ReadScaledPixels(channelDescriptor->port, &(rects->WriteRenderingRectangle), &scaling, &memoryDescriptor);
            writeProxyChannelGradient(&localState, rects, helpBuffer, true, transparencyChannel, proxyPixelSize, transparencyChannel, stride, destination);
        }
        else
        {
            writeProxyChannelGradient(&localState, rects, helpBuffer, false, transparencyChannel, proxyPixelSize, transparencyChannel, stride, destination);
        }
    }
    else
    {
        if (pStateChanged)
        {
            if (channelDescriptor)
            {
                WriteRectangle(destination, rects->WriteRectangle, proxyPixelSize, transparencyChannel, stride, 0);
                gData->sPSChannelPortsSuite->ReadScaledPixels(channelDescriptor->port, &(rects->WriteRenderingRectangle), &scaling, &memoryDescriptor);
            }
            else
            {
                WriteRectangle(destination, rects->WriteRectangle, proxyPixelSize, transparencyChannel, stride, 255);
            }
        }
    }
}

void WriteProxy(GradientState* grState, ProxyState* pState, bool pStateChanged, uint8* helpBuff, int32 proxyPixelSize, int32 stride, uint8* mergedBuffer, uint8* gradientBuffer, int32 & writeWidth, int32 & writeHeight)
{

    if (ValidateGradientState(grState) != GradientStateNoErr) return;

    RectangleRecord rects;
    GetRectangles(pState, &rects);

    if (pStateChanged)
        WriteProxyMergedChannels(&rects, proxyPixelSize, stride, mergedBuffer);

    WriteProxyGradient(grState, pState, &rects, helpBuff, pStateChanged, proxyPixelSize, stride, gradientBuffer);

    writeWidth  = rects.WriteRectangle.right;
    writeHeight = rects.WriteRectangle.bottom;

}

#pragma endregion


