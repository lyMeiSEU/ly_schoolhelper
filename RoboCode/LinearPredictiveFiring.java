package ly;
import robocode.*;
import robocode.util.*;
import java.awt.geom.Point2D;

public class LinearPredictiveFiring extends FiringBase implements IFiringImpl
{
	private double ROBOT_RADIUS = 300;
	
	public void performFiringLogic(AdvancedRobot sourceRobot, TargetRobot targetRobot)
	{
		//System.out.print("Linear Firing Logic\n");
		TargetData targetData = targetRobot.getCurrentTargetData();		
		
		// Start with initial guesses at 10 and 20 ticks		
		double impactTime = getImpactTime(sourceRobot, targetData, 10, 20, 0.01); 
		Point2D.Double impactPoint = getEstimatedPosition(targetData, impactTime);
						
		double bulletHeadingDegrees = calcBulletHeading(sourceRobot, impactPoint);
		double turnAngle = Utils.normalRelativeAngleDegrees
			(bulletHeadingDegrees - sourceRobot.getGunHeading());
			
		//System.out.print("Turn Angle: " + turnAngle + "\n");
		sourceRobot.setTurnGunRight(turnAngle);
		
		double angleThreshold = calcAngleThreshold(targetRobot.getCurrentTargetData());
		//System.out.print("Angle threshold: " + angleThreshold + " Turn Angle: " + turnAngle + "\n");
		if (Math.abs(turnAngle) <= angleThreshold) {
  			// Ensure that the gun is pointing at the correct angle
  			if (
    			(impactPoint.x > 0) &&
 				(impactPoint.x < sourceRobot.getBattleFieldWidth()) &&
 				(impactPoint.y > 0) &&
 				(impactPoint.y < sourceRobot.getBattleFieldHeight())
			) {
    			// Ensure that the predicted impact point is within 
    			// the battlefield
    			sourceRobot.fire(getBulletPower(sourceRobot, targetData));
  			}
		}		
	}

	protected double calcAngleThreshold(TargetData targetData)
	{
		return Math.toDegrees(Math.atan(ROBOT_RADIUS/targetData.getDistance()));
	}

	protected double calcBulletHeading(AdvancedRobot sourceRobot, Point2D.Double impactPoint)
	{
		double dX = (impactPoint.getX() - sourceRobot.getX());
		double dY = (impactPoint.getY() - sourceRobot.getY());

		double distance = Math.sqrt(dX*dX+dY*dY);

		return Math.toDegrees(Math.atan2(dX,dY));												
	}

	protected Point2D.Double getEstimatedPosition(TargetData targetData, double time)
	{
		double x = targetData.getLocation().getX() + 
		   	targetData.getVelocity() * time * Math.sin(Math.toRadians(targetData.getHeading()));
		double y = targetData.getLocation().getY() + 
   			targetData.getVelocity() * time * Math.cos(Math.toRadians(targetData.getHeading()));
		return new Point2D.Double(x,y);	
	}

	protected double getImpactTime(AdvancedRobot sourceRobot, TargetData targetData, double t0, double t1, double accuracy) {

		double X = t1;
		double lastX = t0;
		int iterationCount = 0;
		double lastfX = BulletAccuracy(sourceRobot, targetData, lastX);

		while ((Math.abs(X - lastX) >= accuracy) && (iterationCount < 15)) {

			iterationCount++;
			double fX = BulletAccuracy(sourceRobot, targetData, X);

			if ((fX-lastfX) == 0.0) break;

			double nextX = X - fX*(X-lastX)/(fX-lastfX);
			lastX = X;
			X = nextX;
			lastfX = fX;
		}

		return X;
	}

	private double BulletAccuracy(AdvancedRobot sourceRobot, TargetData targetData, double time)
	{
		double bulletVelocity = 20-3* getBulletPower(sourceRobot, targetData);

		Point2D targetPosition = getEstimatedPosition(targetData, time);
		double dX = (targetPosition.getX() - sourceRobot.getX());
		double dY = (targetPosition.getY() - sourceRobot.getY());

		return Math.sqrt(dX*dX + dY*dY) - bulletVelocity * time;
	}						
														
}
