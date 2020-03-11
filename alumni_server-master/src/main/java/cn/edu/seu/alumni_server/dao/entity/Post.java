package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import javax.persistence.*;

@Data
public class Post implements Serializable {
    @Column(name = "post_id")
    private Long postId;

    @Column(name = "account_id")
    private Long accountId;

    private Integer type;

    @Column(name = "job_name")
    private String jobName;

    private String industry;

    @Column(name = "company_name")
    private String companyName;

    private String province;

    private String city;

    @Column(name = "salary_range")
    private Integer salaryRange;

    @Column(name = "job_detail")
    private String jobDetail;

    private String img1;

    private String img2;

    private String img3;

    private String img4;

    private String img5;

    private String img6;

    @Column(name = "c_time")
    private Date cTime;

    @Column(name = "u_time")
    private Date uTime;

    @Column(name = "valid_status")
    private Boolean validStatus;

    private static final long serialVersionUID = 1L;

}