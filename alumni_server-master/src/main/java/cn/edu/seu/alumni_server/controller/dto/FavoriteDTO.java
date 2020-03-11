package cn.edu.seu.alumni_server.controller.dto;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class FavoriteDTO {
    private Long accountId;
    private Long favoriteAccountId;
    private Integer status;
    private String avatar;
    private String name;
    private String city;
    private String school;
    private String college;
    private String startTime;
    private String company;
    private String position;

    public void setStartTime(final String startTime){
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy");
        this.startTime = dateFormat.format(new Date(Long.parseLong(startTime)));
    }

}
