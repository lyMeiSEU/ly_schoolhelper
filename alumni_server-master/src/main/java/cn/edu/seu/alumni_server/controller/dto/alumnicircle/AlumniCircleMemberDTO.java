package cn.edu.seu.alumni_server.controller.dto.alumnicircle;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;
@Data
@AllArgsConstructor
@NoArgsConstructor
public class AlumniCircleMemberDTO {

    private Long accountId;
    private String name;
    private String avatar;
    private String company;
    private String position;
    private String college;
    private String school;
    private Long startTime;

}
