package cn.edu.seu.alumni_server.controller.dto;

import lombok.Data;

import java.text.SimpleDateFormat;
import java.util.Date;

@Data
public class BriefInfo {
    private Long accountId;
    private String name;
    private String city;
    private String avatar;
    private String selfDesc;
    private String company;
    private String position;
    private String school;
    private String college;
    private String startTime;

    private String recommondReason;

    public void setStartTime(final String startTime){
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy");
        this.startTime = dateFormat.format(new Date(Long.parseLong(startTime)));
    }
}
