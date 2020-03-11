package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.controller.dto.enums.SearchType;
import lombok.Data;

import java.util.List;

@Data
public class SearchResultDTO {
    private long count;
    private SearchType type;
    private List<BriefInfo> list;

    public SearchResultDTO(long count, SearchType type, List<BriefInfo> list) {
        this.count = count;
        this.type = type;
        this.list = list;
    }
}