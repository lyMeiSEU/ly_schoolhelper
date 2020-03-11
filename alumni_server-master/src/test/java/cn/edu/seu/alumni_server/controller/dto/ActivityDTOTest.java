package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Activity;
import org.junit.Test;

import java.util.Date;

import static org.junit.Assert.*;

public class ActivityDTOTest {

    @Test
    public void toActivity() {
        Activity activity = new Activity();
        activity.setActivityTime(new Date());
        activity.setValidStatus(true);
        System.out.println("原来的活动对象");
        System.out.println(activity);
        ActivityDTO activityDTO = new ActivityDTO(activity);
        System.out.println("转化为数据对象");
        System.out.println(activityDTO);
        System.out.println("转换回活动对象");
        System.out.println(activityDTO.toActivity());
    }
}