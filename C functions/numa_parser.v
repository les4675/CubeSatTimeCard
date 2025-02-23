module numa_parser (
    input wire clk,
    input wire rst,
    input wire [7:0] data_in,
    input wire data_valid,
    output reg [31:0] latitude,
    output reg [31:0] longitude,
    output reg [31:0] timestamp,
    output reg parsed_ready
);

    reg [7:0] buffer [0:255];
    reg [7:0] index;
    
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            index <= 0;
            parsed_ready <= 0;
        end else if (data_valid) begin
            buffer[index] <= data_in;
            index <= index + 1;
        end
    end
    
    always @(posedge clk) begin
        if (index >= 12) begin // Assuming 12 bytes needed for parsing
            latitude <= {buffer[0], buffer[1], buffer[2], buffer[3]};
            longitude <= {buffer[4], buffer[5], buffer[6], buffer[7]};
            timestamp <= {buffer[8], buffer[9], buffer[10], buffer[11]};
            parsed_ready <= 1;
            index <= 0;
        end
    end
endmodule
