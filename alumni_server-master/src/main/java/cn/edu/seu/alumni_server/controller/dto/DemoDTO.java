package cn.edu.seu.alumni_server.controller.dto;


import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class DemoDTO {

    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ss")
    private Date demo;

}

