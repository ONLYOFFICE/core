using System;
using System.Collections.Generic;
using System.Text;

namespace OdfConverter.Transforms
{

    public class OoxTransform
    {
        TransformationMatrix _transform;
        // Bouding box
        private double _x, _y, _cx, _cy;
        // Scale
        private double _scaleX;
        private double _scaleY;
        // Rotation
        private double _rot;
        // Flips
        private double _flipH;
        private double _flipV;

        #region Properties
        public double X
        {
            get
            {
                return _x;
            }
        }
        public double Y
        {
            get
            {
                return _y;
            }
        }
        public double CX
        {
            get
            {
                return _cx;
            }
        }
        public double CY
        {
            get
            {
                return _cy;
            }
        }

        public double ScaleX
        {
            get
            {
                return _scaleX;
            }
        }
        public double ScaleY
        {
            get
            {
                return _scaleY;
            }
        }
        public double Rot
        {
            get
            {
                return _rot;
            }
        }
        public double FlipX
        {
            get
            {
                return _flipH;
            }
        }
        public double FlipY
        {
            get
            {
                return _flipV;
            }
        }
        #endregion



        public OoxTransform(long xmlX, long xmlY, long xmlCx, long xmlCy, long xmlRot, long xmlFlipH, long xmlFlipV)
            : this(xmlX, xmlY, xmlCx, xmlCy, xmlX, xmlY, xmlCx, xmlCy, xmlRot, xmlFlipH, xmlFlipV)
        {
        }

        public OoxTransform(long xmlChx, long xmlChy, long xmlChcx, long xmlChcy,
                            long xmlX, long xmlY, long xmlCx, long xmlCy,
                              long xmlRot, long xmlFlipH, long xmlFlipV)
        {
            // 1. Scale the values
            double chx = ((double)xmlChx) / 360000.0;
            double chy = ((double)xmlChy) / 360000.0;
            double chcx = ((double)xmlChcx) / 360000.0;
            double chcy = ((double)xmlChcy) / 360000.0;
            double x = ((double)xmlX) / 360000.0;
            double y = ((double)xmlY) / 360000.0;
            double cx = ((double)xmlCx) / 360000.0;
            double cy = ((double)xmlCy) / 360000.0;
            double rot = ((double)xmlRot) / 60000.0 * (Math.PI / 180.0);
            double flipH = (double)xmlFlipH;
            double flipV = (double)xmlFlipV;

            // 2. Use them
            _x = chx;
            _y = chy;
            _cx = chcx;
            _cy = chcy;
            _scaleX = SafeScale(cx, chcx);
            _scaleY = SafeScale(cy, chcy);
            _rot = rot;
            _flipH = flipH;
            _flipV = flipV;
            CreateMatrix(chx, chy, chcx, chcy, x, y, cx, cy, rot, _flipH, _flipV);
        }

        private const double TwoPI = 2.0 * Math.PI;
        public static OoxTransform CreateLine(long xmlX, long xmlY, long xmlCx, long xmlCy, long xmlRot, long xmlFlipH, long xmlFlipV)
        {
            OoxTransform tmp = new OoxTransform(xmlX, xmlY, xmlCx, xmlCy, xmlRot, xmlFlipH, xmlFlipV);
            double x1, y1;
            tmp.Transform(tmp._x, tmp._y, out x1, out y1);
            double x2, y2;
            tmp.Transform(tmp._x + tmp._cx, tmp._y + tmp._cy, out x2, out y2);
            long newX1 = (long)(x1 * 360000.0);
            long newY1 = (long)(y1 * 360000.0);
            long newX2 = ((long)(x2 * 360000.0));
            long newY2 = ((long)(y2 * 360000.0));
            long newFlipH = 1;
            long newFlipV = 1;
            if (newX1 > newX2)
            {
                long tmpX = newX2;
                newX2 = newX1;
                newX1 = tmpX;
                newFlipH = -1;
            }
            if (newY1 > newY2)
            {
                long tmpY = newY2;
                newY2 = newY1;
                newY1 = tmpY;
                newFlipV = -1;
            }
            return new OoxTransform(newX1, newY1, newX2 - newX1, newY2 - newY1, 0, newFlipH, newFlipV);
        }
        private double Normalize(double angle)
        {
            if (angle >= 0)
            {
                // First, take a "modulo"
                angle -= TwoPI * (int)(angle / TwoPI);
                // Now finish the normalization
                if (angle >= Math.PI)
                {
                    angle -= TwoPI;
                }
            }
            else
            {
                angle += TwoPI * (int)((-angle) / TwoPI);
                if (angle < -Math.PI)
                {
                    angle += TwoPI;
                }
            }
            return angle;
        }
        private static double SafeScale(double num, double denom)
        {
            if (denom == 0.0)
            {
                return 1;
            }
            else
            {
                return num / denom;
            }
        }
        public OoxTransform(OoxTransform outer, OoxTransform inner)
        {
            // Get the Bounding box of the most inner transformation
            _x = inner.X;
            _y = inner.Y;
            _cx = inner.CX;
            _cy = inner.CY;
            // Now combine all transformations (5.7.4, "Render a simple shape inside a group hierarchy", steps 1, 2, 3)
            _rot = Normalize(outer._rot + inner._rot);
            _flipH = outer._flipH * inner._flipH;
            _flipV = outer._flipV * inner._flipV;
            _scaleX = outer._scaleX * inner._scaleX;
            _scaleY = outer._scaleY * inner._scaleY;
            TransformationMatrix m = outer._transform * inner._transform;   // Note : the center should be invariant by inner._transform...
            // Calculate the coordinates of the new Center (5.7.4, "Render a simple shape inside a group hierarchy", step 4)
            PointCoordinates originalCenter = new PointCoordinates(_x + _cx / 2.0, _y + _cy / 2.0);
            PointCoordinates newCenter = m.Transform(originalCenter);
            // Now calculate the target "Bounding box"
            double newCx = _cx * _scaleX;
            double newCy = _cy * _scaleY;
            double newX = newCenter.X - newCx / 2.0;
            double newY = newCenter.Y - newCy / 2.0;
            // And finally compute the transformation matrix...
            CreateMatrix(_x, _y, _cx, _cy, newX, newY, newCx, newCy, _rot, _flipH, _flipV);
        }

        private void CreateMatrix(double Bx, double By, double Dx, double Dy, double B1x, double B1y,
                            double D1x, double D1y, double teta, double Fx, double Fy)
        {

            TransformationMatrix scaleTranslate = new TransformationMatrix(
                 SafeScale(D1x, Dx), 0.0, B1x - SafeScale(D1x, Dx) * Bx,
                 0.0, SafeScale(D1y, Dy), B1y - SafeScale(D1y, Dy) * By,
                0.0, 0.0, 1.0);

            TransformationMatrix U = new TransformationMatrix(
                1.0, 0.0, -(B1x + D1x / 2.0),
                0.0, 1.0, -(B1y + D1y / 2.0),
                0.0, 0.0, 1.0);

            TransformationMatrix U1 = new TransformationMatrix(
                1.0, 0.0, (B1x + D1x / 2.0),
                0.0, 1.0, (B1y + D1y / 2.0),
                0.0, 0.0, 1.0);

            TransformationMatrix Tmp1 = new TransformationMatrix(
                Math.Cos(teta), -Math.Sin(teta), 0.0,
                Math.Sin(teta), Math.Cos(teta), 0.0,
                0.0, 0.0, 1.0);
            TransformationMatrix Tmp2 = new TransformationMatrix(
                Fx, 0.0, 0.0,
                0.0, Fy, 0.0,
                0.0, 0.0, 1.0);
            TransformationMatrix rotate = U1 * Tmp1 * Tmp2 * U;
            _transform = rotate * scaleTranslate;
        }

        public void Transform(double x, double y, out double newX, out double newY)
        {
            PointCoordinates sourcePt = new PointCoordinates(x, y);
            PointCoordinates newPt = _transform.Transform(sourcePt);
            newX = newPt.X;
            newY = newPt.Y;
        }

        public virtual string Odf
        {
            get
            {
                double x, y;
                Transform(_x, _y, out x, out y);
                if (_rot == 0.0)
                {
                    //   return string.Format("svg:width={0:F3}cm@svg:height={1:F3}cm:svg:x={2:F3}cm svg:y={3:F3}cm",
                    return string.Format(System.Globalization.CultureInfo.InvariantCulture, "NOROT@{0:F3}cm@{1:F3}cm@{2:F3}cm@{3:F3}cm",
                        _cx * _scaleX, _cy * _scaleY, x, y);
                }
                else
                {
                    // Compute the new position of (_x,_y) point
                    return string.Format(System.Globalization.CultureInfo.InvariantCulture, "YESROT@{0:F3}cm@{1:F3}cm@rotate ({2}) translate ({3:F3}cm {4:F3}cm)",
                        _cx * _scaleX, _cy * _scaleY, -_rot, x, y);
                }
            }
        }
        public virtual string LineOdf
        {
            get
            {
                double x1, y1;
                Transform(_x, _y, out x1, out y1);
                double x2, y2;
                Transform(_x + _cx, _y + _cy, out x2, out y2);
                return string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:F3}cm@{1:F3}cm@{2:F3}cm@{3:F3}cm",
                    x2, y2, x1, y1);

            }
        }
    }

}
