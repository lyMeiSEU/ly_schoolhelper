package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Table;
import java.io.Serializable;

@Data
@Table(name = "const_major")
public class ConstMajor implements Serializable {
    @Column(name = "major_id")
    private Integer majorId;

    @Column(name = "major_name")
    private String majorName;

    private static final long serialVersionUID = 1L;

}