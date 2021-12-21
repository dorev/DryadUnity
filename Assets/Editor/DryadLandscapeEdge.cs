using System;
using UnityEditor;
using UnityEngine;

public class DryadLandscapeEdge
{
    public DryadLandscapeNode sourceNode;
    public DryadLandscapeNode destinationNode;
    public Action<DryadLandscapeEdge> OnRemoveEdge;

    public DryadLandscapeEdge(DryadLandscapeNode sourceNode, DryadLandscapeNode destinationNode, Action<DryadLandscapeEdge> OnClickRemoveLink)
    {
        this.sourceNode = sourceNode;
        this.destinationNode = destinationNode;
        this.OnRemoveEdge = OnClickRemoveLink;
    }

    public static bool LineIntersection(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, out Vector2 intersection)
    {
        float Ax, Bx, Cx, Ay, By, Cy, d, e, f, num;
        float x1lo, x1hi, y1lo, y1hi;
        intersection.x = 0;
        intersection.y = 0;

        Ax = p2.x - p1.x;
        Bx = p3.x - p4.x;

        // X bound box test
        if (Ax < 0)
        {
            x1lo = p2.x;
            x1hi = p1.x;
        }
        else
        {
            x1hi = p2.x;
            x1lo = p1.x;
        }

        if (Bx > 0)
        {
            if (x1hi < p4.x || p3.x < x1lo)
                return false;
        }
        else
        {
            if (x1hi < p3.x || p4.x < x1lo)
                return false;
        }

        Ay = p2.y - p1.y;
        By = p3.y - p4.y;

        // Y bound box test
        if (Ay < 0)
        {
            y1lo = p2.y;
            y1hi = p1.y;
        }
        else
        {
            y1hi = p2.y;
            y1lo = p1.y;
        }

        if (By > 0)
        {
            if (y1hi < p4.y || p3.y < y1lo)
                return false;
        }
        else
        {
            if (y1hi < p3.y || p4.y < y1lo)
                return false;
        }

        Cx = p1.x - p3.x;
        Cy = p1.y - p3.y;
        d = By * Cx - Bx * Cy;  // alpha numerator
        f = Ay * Bx - Ax * By;  // both denominator

        // alpha tests
        if (f > 0)
        {
            if (d < 0 || d > f)
                return false;
        }
        else
        {
            if (d > 0 || d < f)
                return false;
        }

        e = Ax * Cy - Ay * Cx;  // beta numerator

        // beta tests
        if (f > 0)
        {
            if (e < 0 || e > f)
                return false;
        }
        else
        {
            if (e > 0 || e < f)
                return false;
        }

        // check if they are parallel
        if (f == 0)
            return false;

        // compute intersection coordinates
        num = d * Ax; // numerator
        intersection.x = p1.x + num / f;
        num = d * Ay;
        intersection.y = p1.y + num / f;

        return true;
    }


    Vector2 DestinationNodeLinkIntersection()
    {
        Rect sourceNode = this.sourceNode.Rect;
        Rect destinationNode = this.destinationNode.Rect;

        Vector2 topSideCrossingPoint;
        LineIntersection(
            sourceNode.center,
            destinationNode.center,
            new Vector2(destinationNode.x, destinationNode.y),
            new Vector2(destinationNode.xMax, destinationNode.y),
            out topSideCrossingPoint);

        Vector2 bottomSideCrossingPoint;
        LineIntersection(
            sourceNode.center,
            destinationNode.center,
            new Vector2(destinationNode.x, destinationNode.yMax),
            new Vector2(destinationNode.xMax, destinationNode.yMax),
            out bottomSideCrossingPoint);

        Vector2 leftSideCrossingPoint;
        LineIntersection(
            sourceNode.center,
            destinationNode.center,
            new Vector2(destinationNode.x, destinationNode.y),
            new Vector2(destinationNode.x, destinationNode.yMax),
            out leftSideCrossingPoint);

        Vector2 rightSideCrossingPoint;
        LineIntersection(
            sourceNode.center,
            destinationNode.center,
            new Vector2(destinationNode.xMax, destinationNode.y),
            new Vector2(destinationNode.xMax, destinationNode.yMax),
            out rightSideCrossingPoint);

        Vector2 closestPoint = new Vector2(float.MaxValue, float.MaxValue);
        float closestDistance = float.MaxValue;
        float distance;

        if (topSideCrossingPoint != Vector2.zero)
        {
            distance = Vector2.Distance(sourceNode.center, topSideCrossingPoint);
            if (distance < closestDistance)
            {
                closestPoint = topSideCrossingPoint;
                closestDistance = distance;
            }
        }

        if (bottomSideCrossingPoint != Vector2.zero)
        {
            distance = Vector2.Distance(sourceNode.center, bottomSideCrossingPoint);
            if (distance < closestDistance)
            {
                closestPoint = bottomSideCrossingPoint;
                closestDistance = distance;
            }
        }

        if (leftSideCrossingPoint != Vector2.zero)
        {
            distance = Vector2.Distance(sourceNode.center, leftSideCrossingPoint);
            if (distance < closestDistance)
            {
                closestPoint = leftSideCrossingPoint;
                closestDistance = distance;
            }
        }
        
        if (rightSideCrossingPoint != Vector2.zero)
        {
            distance = Vector2.Distance(sourceNode.center, rightSideCrossingPoint);
            if (distance < closestDistance)
                closestPoint = rightSideCrossingPoint;
        }

        return closestPoint;
    }

    public void Draw()
    {
        Vector2 linkEndPoint = DestinationNodeLinkIntersection();

        Handles.DrawLine(sourceNode.Rect.center, linkEndPoint, 1f);

        if(Handles.Button(linkEndPoint, Quaternion.identity, 4, 8, Handles.RectangleHandleCap) && OnRemoveEdge != null)
            OnRemoveEdge(this);
    }
}
