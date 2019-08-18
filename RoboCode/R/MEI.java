package LY;

import robocode.*;
import java.awt.Color;

public class MEI extends AdvancedRobot{
	
   final double pi = Math.PI;

   long inCorner; // 撞墙的时间
   Enemy Etarget; // 目标敌机
   int hit = 0; // 中枪次数
   int direcao = 1; //方向指示
   double potencia; //功率指示
   
	void calculoMovimento() {
        // 如果敌机距离超过300，以45度角接近
        if (Etarget.distance < 300) {
            // 最后动作已经过去一段时间
            if (getTime() % 20 == 0) {
                if (hit < 4) {  // 命中次数小于4，改变运动方向
                    direcao*= -1;
                } else {
                     if (getTime() % 60 == 0) { // 一分钟内未被击中
                        hit = 0;
                    }
                }
                setAhead(direcao* (350 +(int)((int) Math.random() * 350))); //mudamos a rota bb;  
            }
            //改变运动路线
            setTurnRightRadians(Etarget.bearing + (pi / 2));
        } else {
            setAhead(300);
            setTurnRightRadians(Etarget.bearing + (pi / 4));
        }
    } // 动作结束



    void escanear() {
	
        double radarOffset;
     
	   if (getTime() - Etarget.ctime > 5) {
            radarOffset = 360;  
        } else {
           // 计算敌人位置用于瞄准, 设计延时避免敌人改变路线
            radarOffset = getRadarHeadingRadians() - absbearing(getX(), getY(), Etarget.x, Etarget.y);

            if (radarOffset < 0) {
                radarOffset -= pi / 7;
            } else {
                radarOffset += pi / 7;
            }
        }
        // 改变雷达位置
        setTurnRadarLeftRadians(NormaliseBearing(radarOffset));
	}


    public void run() {
	
		setColors(Color.BLACK, Color.WHITE, Color.BLACK);//cor 

        // 变量初始化
        inCorner = getTime(); // 获得游戏时间
        Etarget = new Enemy(); // 初始化目标（对准敌机）
		
        Etarget.distance = 900000000; // 初始化距离为无穷远

        setAdjustGunForRobotTurn(true); // 设置武器调整为真
        setAdjustRadarForGunTurn(true); // 设置雷达调整为真
        turnRadarRightRadians(2 * pi); //Calculos by Heber e sua IA
        
		while (true) { // 主循环
            
            calculoMovimento(); // 计算下一步行动

            calculoPotencia(); // 计算枪的功率

            escanear();// 扫描敌人
            apontar(); // 瞄准敌人
            setFire(potencia);// 计算并开炮
            execute();
        } // end loop
    } //  end main


    // 计算被枪击次数
    public void onHitByBullet(HitByBulletEvent event) {
        hit = hit + 1;
    }

    // 防撞墙操作
    public void onHitWall(HitWallEvent event) {

        long temp = getTime();
        // 获取上次碰撞时间

        if ((temp - inCorner) < 100) {
            setBack(100);
            setTurnRightRadians(Etarget.bearing);
            execute();
            setAhead(300);
            execute();
        }
        inCorner = temp;
    }

    // 计算大炮瞄准
    void apontar() {
     
        long time = getTime() + (int) (Etarget.distance / (20 - (3 * (400 / Etarget.distance))));
      
        double gunOffset = getGunHeadingRadians() - absbearing(getX(), getY(), Etarget.guessX(time), Etarget.guessY(time));
        setTurnGunLeftRadians(NormaliseBearing(gunOffset));
    }

    // 计算雷达朝向
    double NormaliseBearing(double ang) {
        if (ang > pi) {
            ang -= 2 * pi;
        }
        if (ang < -pi) {
            ang += 2 * pi;
        }
        return ang;
    }

    
    public double distancia(double x1, double y1, double x2, double y2) {
        return Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }
    
    public double absbearing(double x1, double y1, double x2, double y2) {
        double xo = x2 - x1;
        double yo = y2 - y1;
        double h = distancia(x1, y1, x2, y2);
        if (xo > 0 && yo > 0) {
            return Math.asin(xo / h);
        }
        if (xo > 0 && yo < 0) {
            return Math.PI - Math.asin(xo / h);
        }
        if (xo < 0 && yo < 0) {
            return Math.PI + Math.asin(-xo / h);
        }
        if (xo < 0 && yo > 0) {
            return 2.0 * Math.PI - Math.asin(-xo / h);
        }
        return 0;
    }
    
    public void onScannedRobot(ScannedRobotEvent e) {

        System.out.println("Find enermy :" + e.getName());

        if ((e.getDistance() < Etarget.distance) || (Etarget.name == e.getName())) { // atira e depois 
            // 获取敌机参数
            Etarget.name = e.getName();
            Etarget.bearing = e.getBearingRadians();
            Etarget.head = e.getHeadingRadians();
            Etarget.ctime = getTime();
            Etarget.speed = e.getVelocity();
            Etarget.distance = e.getDistance();
            double absbearing_rad = (getHeadingRadians() + e.getBearingRadians()) % (2 * pi);
            Etarget.x = getX() + Math.sin(absbearing_rad) * e.getDistance();
            Etarget.y = getY() + Math.cos(absbearing_rad) * e.getDistance();
        }

    }

    // 计算所需能量(与距离成反比)
    void calculoPotencia() {
        potencia = 500 / Etarget.distance;
    }

    // 机器人被击毙，调整参数
    public void onRobotDeath(RobotDeathEvent e) {
        if (e.getName() == Etarget.name) {
            Etarget.distance = 9000000;
        }
    }


    // 受到伤害便攻击
    public void onHitRobot(HitRobotEvent event) {   
	    if (event.getName() != Etarget.name) {
            Etarget.distance = 9000000;
        }
    }
}

class Enemy {

    String name;
    public double bearing;
    public double head;
    public long ctime;
    public double speed;
    public double x, y;
    public double distance;

    public String getname() {
        return name;
    }

    public double getbearing() {
        return bearing;
    }

    public double gethead() {
        return head;
    }

    public long getctime() {
        return ctime;
    }

    public double getspeed() {
        return speed;
    }

    public double getx() {
        return x;
    }

    public double gety() {
        return y;
    }

    public double getdistance() {
        return distance;
    }

    public double guessX(long when) {
        long diff = when - ctime;
        return x + Math.sin(head) * speed * diff;
    }

    public double guessY(long when) {
        long diff = when - ctime;
        return y + Math.cos(head) * speed * diff;
    }
}

