class ChangeDataTypeForBidValue < ActiveRecord::Migration
  
  def up
    change_table :bids do |t|
      t.change :value, :float
    end
  end

  def down
    change_table :bids do |t|
      t.change :value, :integer
    end
  end

end
