package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Date;

@Data
public class Job implements Serializable {
    @Column(name = "account_id")
    private Long accountId;

    @Column(name = "job_id")
    @Id
    private Long jobId;

    private String company;

    private String position;

    @Column(name = "start_time")
    private Long startTime;

    @Column(name = "end_time")
    private Long endTime;

    @Column(name = "c_time", insertable = false)
    private Date cTime;

    /**
     * update
     */
    @Column(name = "u_time", insertable = false)
    private Date uTime;

    @Column(name = "valid_status", insertable = false)
    private Boolean validStatus;

    private static final long serialVersionUID = 1L;

}