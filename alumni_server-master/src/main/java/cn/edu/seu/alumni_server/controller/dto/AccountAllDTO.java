package cn.edu.seu.alumni_server.controller.dto;

import lombok.Data;

import javax.validation.constraints.NotNull;
import java.util.List;

@Data
public class AccountAllDTO {
    @NotNull
    private AccountDTO account;
    @NotNull
    private List<EducationDTO> educations;
    @NotNull
    private List<JobDTO> jobs;

    private List<FriendDTO> friends;

    /**
     * 供前台展示两人关系用
     * 本人 null
     * 其他状态 FriendStatus
     */
    private Integer relationShip;
    /**
     *
     */
    private Integer favorite;

}
