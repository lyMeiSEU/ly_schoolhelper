package ly;
import robocode.*;
import java.util.*;
import robocode.util.*;
import java.awt.geom.Point2D;

public class TargetData
{
	private double _bearing;
	private double _distance;
	private double _heading;
	private Point2D.Double _location;
	private long _time;
	private double _velocity;
	public TargetData(ScannedRobotEvent scanEvent, Point2D.Double targetLocation )
	{
		_bearing = scanEvent.getBearing();
		_distance = scanEvent.getDistance();
		_heading = scanEvent.getHeading();
		_location = targetLocation;
		_time = scanEvent.getTime();
		_velocity = scanEvent.getVelocity();
	}

	public double getBearing()
	{
		return _bearing;
	}

	public double getDistance()
	{
		return _distance;
	}

	public double getHeading()
	{
		return _heading;
	}

	public Point2D.Double getLocation()
	{
		return _location;
	}

	public long getTime()
	{
		return _time;
	}

	public double getVelocity()
	{
		return _velocity;
	}
}
