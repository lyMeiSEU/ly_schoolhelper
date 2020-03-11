package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.ConstMajor;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class ConstMajorDTO {

    private Integer majorId;

    private String majorName;

    public ConstMajorDTO(){}

    public ConstMajorDTO(ConstMajor constMajor){
        BeanUtils.copyProperties(constMajor, this);
    }

}
