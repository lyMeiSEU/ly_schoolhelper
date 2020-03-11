package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.ConstSchool;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class ConstSchoolDTO {

    private Integer schoolId;

    private String schoolName;

    public ConstSchoolDTO(){}

    public ConstSchoolDTO(ConstSchool constSchool) {
        BeanUtils.copyProperties(constSchool, this);
    }
}
