package ly;
import robocode.*;
import java.util.*;
import robocode.util.*;
import java.awt.geom.Point2D;

public class PatternPredictiveFiring extends FiringBase implements IFiringImpl
{
	PatternTracker _tracker;
	public PatternPredictiveFiring(PatternTracker tracker)
	{
		_tracker = tracker;
	}

	public void performFiringLogic(AdvancedRobot sourceRobot, TargetRobot targetRobot)
	{
		double bulletPower = getBulletPower(sourceRobot, targetRobot.getCurrentTargetData());
		long patternIndex = _tracker.findSimilarPeriodEndIndex(sourceRobot, targetRobot.getCurrentTargetData(), bulletPower);
		
		if (patternIndex > 0)
		{
			double targetBearing = _tracker.calcProjectedBearing(sourceRobot, targetRobot, bulletPower, patternIndex);
			sourceRobot.setTurnGunRightRadians(Utils.normalRelativeAngle(targetBearing - sourceRobot.getGunHeadingRadians()));
			sourceRobot.setFire(bulletPower);
		}
		else
		{
			LinearPredictiveFiring firing = new LinearPredictiveFiring();
			firing.performFiringLogic(sourceRobot, targetRobot);
		}
	}
}
