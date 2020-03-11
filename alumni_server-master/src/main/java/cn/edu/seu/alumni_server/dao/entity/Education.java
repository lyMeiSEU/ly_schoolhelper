package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import javax.annotation.security.DenyAll;
import javax.persistence.*;

@Data
public class Education implements Serializable {
    @Column(name = "account_id")
    private Long accountId;

    @Column(name = "education_id")
    @Id
    private Long educationId;

    private String education;

    private String school;

    private String college;

    @Column(name = "start_time")
    private Long startTime;

    @Column(name = "end_time")
    private Long endTime;

    @Column(name = "c_time", insertable = false)
    private Date cTime;

    @Column(name = "u_time", insertable = false)
    private Date uTime;

    @Column(name = "valid_status", insertable = false)
    private Boolean validStatus;

    private static final long serialVersionUID = 1L;
}