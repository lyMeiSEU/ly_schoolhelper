package ly;
import robocode.*;
import java.util.*;
import robocode.util.*;
import java.awt.geom.Point2D;

public class FiringBase
{
	protected double getBulletPower(AdvancedRobot sourceRobot, TargetData targetData)
	{
		double distance = targetData.getDistance();
		double power;
		if (distance > 300 || (sourceRobot.getEnergy() < 40 && distance > 100))
		{
			power = 1;
		}
		else if (distance < 50)
		{
			power = 3;
		}
		else
		{
			power = 3 - ((distance-50) * .008);  // 提供与目标距离负相关的火力大小
		}

		System.out.print("Distance: " + distance + " Power: " + power + "\n");
		return power;							
	}

														
}
