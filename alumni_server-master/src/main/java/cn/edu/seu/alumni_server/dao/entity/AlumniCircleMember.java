package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import java.io.Serializable;

@Data
public class AlumniCircleMember implements Serializable {

    private Long alumniCircleId;

    private Long accountId;
    @Column(name = "valid_status", insertable = false)
    private Boolean validStatus;

    private Boolean pushStatus;

    private static final long serialVersionUID = 1L;
}
