package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import javax.persistence.*;
@Data
@Table(name = "const_school")
public class ConstSchool implements Serializable {
    @Column(name = "school_id")
    private Integer schoolId;

    @Column(name = "school_name")
    private String schoolName;

    private static final long serialVersionUID = 1L;

}